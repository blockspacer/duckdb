//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/join/physical_delim_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/types/chunk_collection.hpp"
#include "duckdb/execution/physical_sink.hpp"

namespace duckdb {
class PhysicalHashAggregate;

//! PhysicalDelimJoin represents a join where the LHS will be duplicate eliminated and pushed into a
//! PhysicalChunkCollectionScan in the RHS.
class PhysicalDelimJoin : public PhysicalSink {
public:
	PhysicalDelimJoin(vector<TypeId> types, unique_ptr<PhysicalOperator> original_join,
	                  vector<PhysicalOperator *> delim_scans);

	unique_ptr<PhysicalOperator> join;
	unique_ptr<PhysicalHashAggregate> distinct;
	ChunkCollection lhs_data;
	ChunkCollection delim_data;

public:
	unique_ptr<GlobalOperatorState> GetGlobalState(ClientContext &context) override;
	unique_ptr<LocalSinkState> GetLocalSinkState(ExecutionContext &context) override;
	void Sink(ExecutionContext &context, GlobalOperatorState &state, LocalSinkState &lstate, DataChunk &input) override;
	void Finalize(ClientContext &context, unique_ptr<GlobalOperatorState> state) override;

	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;

	string ExtraRenderInformation() const override;
};

} // namespace duckdb
