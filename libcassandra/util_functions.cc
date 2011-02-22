/*
 * LibCassandra
 * Copyright (C) 2010 Padraig O'Sullivan
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license. See
 * the COPYING file in the parent directory for full text.
 */

#include <string>
#include <sstream>
#include <iostream>

#include "libcassandra/cassandra_host.h"
#include "libcassandra/util_functions.h"

using namespace std;
using namespace org::apache::cassandra;

namespace libcassandra
{

int parsePortFromURL(const string &url)
{
  size_t pos= url.find_first_of(":");
  int found_port= CassandraHost::DEFAULT_PORT;
  if (pos == string::npos)
  {
    return found_port;
  }
  istringstream port_stream(url.substr(pos + 1));
  port_stream >> found_port;
  return found_port;
}


string parseHostFromURL(const string &url)
{
  size_t pos= url.find_first_of(":");
  if (pos == string::npos)
  {
    return url;
  }
  return url.substr(0, pos);
}


ColumnDef createColumnDefObject(const ColumnDefinition& col_def)
{
  ColumnDef thrift_col_def;
  thrift_col_def.name.assign(col_def.getName());
  thrift_col_def.validation_class.assign(col_def.getValidationClass());
  return thrift_col_def;
}


KsDef createKsDefObject(const KeyspaceDefinition& ks_def)
{
  KsDef thrift_ks_def;
  thrift_ks_def.name.assign(ks_def.getName());
  thrift_ks_def.strategy_class.assign(ks_def.getStrategyClass());
  vector<ColumnFamilyDefinition> cf_defs= ks_def.getColumnFamilies();
  for (vector<ColumnFamilyDefinition>::iterator it= cf_defs.begin();
       it != cf_defs.end();
       ++it)
  {
    CfDef entry= createCfDefObject(*it);
    thrift_ks_def.cf_defs.push_back(entry);
  }
  thrift_ks_def.replication_factor= ks_def.getReplicationFactor();
  return thrift_ks_def;
}


CfDef createCfDefObject(const ColumnFamilyDefinition& cf_def)
{
  CfDef thrift_cf_def;
  /* 
   * keyspace name and cf name are required 
   * TODO - throw an exception if these are not present
   */
  thrift_cf_def.keyspace.assign(cf_def.getKeyspaceName());
  thrift_cf_def.name.assign(cf_def.getName());
  /* everything else associated with a column family is optional */
  if (cf_def.isColumnTypeSet())
  {
    thrift_cf_def.column_type.assign(cf_def.getColumnType());
    thrift_cf_def.__isset.column_type= true;
  }
  if (cf_def.isComparatorTypeSet())
  {
    thrift_cf_def.comparator_type.assign(cf_def.getComparatorType());
    thrift_cf_def.__isset.comparator_type= true;
  }
  if (cf_def.isSubComparatorTypeSet())
  {
    thrift_cf_def.subcomparator_type.assign(cf_def.getSubComparatorType());
    thrift_cf_def.__isset.subcomparator_type= true;
  }
  if (cf_def.isCommentSet())
  {
    thrift_cf_def.comment.assign(cf_def.getComment());
    thrift_cf_def.__isset.comment= true;
  }
  if (cf_def.isRowCacheSizeSet())
  {
    thrift_cf_def.row_cache_size= cf_def.getRowCacheSize();
    thrift_cf_def.__isset.row_cache_size= true;
  }
  if (cf_def.isKeyCacheSizeSet())
  {
    thrift_cf_def.key_cache_size= cf_def.getKeyCacheSize();
    thrift_cf_def.__isset.key_cache_size= true;
  }
  if (cf_def.isReadRepairChanceSet())
  {
    thrift_cf_def.read_repair_chance= cf_def.getReadRepairChance();
    thrift_cf_def.__isset.read_repair_chance= true;
  }
  if (cf_def.isGcGraceSecondsSet())
  {
    thrift_cf_def.gc_grace_seconds= cf_def.getGcGraceSeconds();
    thrift_cf_def.__isset.gc_grace_seconds= true;
  }
  if (cf_def.isDefaultValidationClassSet())
  {
    thrift_cf_def.default_validation_class.assign(cf_def.getDefaultValidationClass());
    thrift_cf_def.__isset.default_validation_class= true;
  }
  if (cf_def.isIdSet())
  {
    thrift_cf_def.id= cf_def.getId();
    thrift_cf_def.__isset.id= true;
  }
  if (cf_def.isMaxCompactionThresholdSet())
  {
    thrift_cf_def.max_compaction_threshold= cf_def.getMaxCompactionThreshold();
    thrift_cf_def.__isset.max_compaction_threshold= true;
  }
  if (cf_def.isMinCompactionThresholdSet())
  {
    thrift_cf_def.min_compaction_threshold= cf_def.getMinCompactionThreshold();
    thrift_cf_def.__isset.min_compaction_threshold= true;
  }
  if (cf_def.isMemtableFlushAfterMinsSet())
  {
    thrift_cf_def.memtable_flush_after_mins= cf_def.getMemtableFlushAfterMins();
    thrift_cf_def.__isset.memtable_flush_after_mins= true;
  }
  if (cf_def.isMemtableOperationsInMillionsSet())
  {
    thrift_cf_def.memtable_operations_in_millions= cf_def.getMemtableOperationsInMillions();
    thrift_cf_def.__isset.memtable_operations_in_millions= true;
  }
  if (cf_def.isMemtableThroughputInMbSet())
  {
    thrift_cf_def.memtable_throughput_in_mb= cf_def.getMemtableThroughputInMb();
    thrift_cf_def.__isset.memtable_throughput_in_mb= cf_def.getMemtableThroughputInMb();
  }
  if (cf_def.isColumnMetadataSet())
  {
    vector<ColumnDefinition> cols= cf_def.getColumnMetadata();
    for (vector<ColumnDefinition>::iterator it= cols.begin();
         it != cols.end();
         ++it)
    {
      ColumnDef thrift_col= createColumnDefObject(*it);
      thrift_cf_def.column_metadata.push_back(thrift_col);
    }
    thrift_cf_def.__isset.column_metadata= true;
  }
  return thrift_cf_def;
}

} /* end namespace libcassandra */
