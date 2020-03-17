/*
 * dataloader.h
 *
 *  Created on: Nov 29, 2018
 *      Author: felipe
 */

#pragma once

#include <FileSystem/Uri.h>
#include "data_parser/DataParser.h"
#include "data_provider/DataProvider.h"
#include <arrow/io/interfaces.h>
#include <blazingdb/manager/Context.h>
#include <vector>

#include <memory>


namespace ral {

namespace io {

namespace {
using blazingdb::manager::experimental::Context;
}  // namespace

/**
 * class is used for loading data from some kind of file type using some kind of file provider
 * in our case we will be using blazing-io to read and write files but a local version could also be made
 */

class data_loader {
public:
	data_loader(std::shared_ptr<data_parser> parser, std::shared_ptr<data_provider> provider);
	data_loader(const data_loader& ) = default;
	std::shared_ptr<data_loader> clone();

	virtual ~data_loader();

	/**
	 * returns data into a std::unique_ptr<ral::frame::BlazingTable>
	 */
	std::unique_ptr<ral::frame::BlazingTable>  load_data(
		Context * context,
		const std::vector<size_t> & column_indices,
		const Schema & schema);

	std::unique_ptr<ral::frame::BlazingTable>  load_batch(
		Context * context,
		const std::vector<size_t> & column_indices_in,
		const Schema & schema,
		std::string user_readable_file_handle,
		data_handle file_data_handle,
		size_t file_index,
		size_t batch_id);
		
	void get_schema(Schema & schema, std::vector<std::pair<std::string, gdf_dtype>> non_file_columns);

	std::unique_ptr<ral::frame::BlazingTable> get_metadata(int offset);

	std::shared_ptr<data_provider> get_provider() {
		return provider;
	}
	std::shared_ptr<data_parser> get_parser() {
		return parser;
	}

private:
	/**
	 * DataProviders are able to serve up one or more arrow::io::RandomAccessFile objects
	 */
	std::shared_ptr<data_provider> provider;
	/**
	 * parsers are able to parse arrow::io::RandomAccessFile objects of a specific file type and convert them into
	 * gdf_column_cpp
	 */
	std::shared_ptr<data_parser> parser;
};


} /* namespace io */
} /* namespace ral */
