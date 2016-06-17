package cachester

// #include "batch.h"
import "C"
import "unsafe"

type bulkBuffer struct {
	recSize C.size_t
	rawPtr  unsafe.Pointer
	numRecs C.size_t
	rawBuff []byte
	records [][]byte
}

type ChangeReader struct {
	bulkBuffer
	head C.q_index
}
type BatchReader struct {
	bulkBuffer
	ids    []int64
	idsPtr *C.identifier
}

func newBulkBuffer(recordSize int, numRecords int) bulkBuffer {
	var bb bulkBuffer
	bb.numRecs = C.size_t(numRecords)
	bb.recSize = C.size_t(recordSize)
	bb.rawBuff = make([]byte, numRecords*recordSize)
	bb.rawPtr = unsafe.Pointer(&bb.rawBuff[0])
	bb.records = make([][]byte, numRecords)
	for i := 0; i < numRecords; i++ {
		start := i * recordSize
		bb.records[i] = bb.rawBuff[start : start+recordSize]
	}
	return bb
}

// NewBatchReader creates a batch reader
func NewBatchReader(recordSize int, ids []int64) *BatchReader {
	return &BatchReader{
		newBulkBuffer(recordSize, len(ids)),
		ids,
		(*C.identifier)(&ids[0]),
	}
}

// NewChangeReader creates a change queue reader
func NewChangeReader(recordSize, numRecords int) *ChangeReader {
	return &ChangeReader{newBulkBuffer(recordSize, numRecords), -1}
}

// Load copies data from the cachester storage to this buffer
func (br *BatchReader) Load(cs *Store) error {
	status := C.batch_read_records(cs.store, br.recSize,
		br.idsPtr, br.rawPtr, nil,
		nil, br.numRecs)
	return call(status)
}
func (bb *bulkBuffer) GetRecord(idx int64) []byte {
	return bb.records[idx]
}
func (bb *bulkBuffer) GetRecordPtr(idx int64) unsafe.Pointer {
	return unsafe.Pointer(&bb.records[idx])
}

func (cs *Store) GetRecords(recordSize int, ids []int64) ([][]byte, error) {
	numRecs := len(ids)
	rawBuff := make([]byte, numRecs*recordSize)
	buffs := make([][]byte, numRecs)
	for i := 0; i < len(buffs); i++ {
		start := i * recordSize
		buffs[i] = rawBuff[start : start+recordSize]
	}
	status := C.batch_read_records(cs.store, C.size_t(recordSize),
		(*C.identifier)(&ids[0]), unsafe.Pointer(&rawBuff[0]), nil,
		nil, C.size_t(numRecs))
	if status < 0 {
		return nil, call(status)
	} else {
		return buffs, nil
	}
}
