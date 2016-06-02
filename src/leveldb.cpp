//' @useDynLib RLevelDB
//' @importFrom Rcpp evalCpp

#include <Rcpp.h>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>

// [[Rcpp::export]]
SEXP cppOpenLevelDB(const std::string &path, const bool create)
{
  // set options
  leveldb::Options options;
  options.create_if_missing = create;

  // open db
  leveldb::DB *db;
  leveldb::Status status = leveldb::DB::Open(options, path, &db);

  // check status
  if(!status.ok()){
    Rcpp::stop("failed to open db.");
  }

  Rcpp::XPtr<leveldb::DB> ptr(db);

  return ptr;
}

// [[Rcpp::export]]
Rcpp::CharacterVector cppDbGet(SEXP xp,
                               const std::vector<std::string> &keys)
{
  Rcpp::XPtr<leveldb::DB> ptr(xp);

  Rcpp::CharacterVector value_vec;
  for(std::vector<std::string>::const_iterator it_key = keys.begin();
      it_key != keys.end(); ++it_key){
    std::string value;
    leveldb::Status status = ptr->Get(leveldb::ReadOptions(),
                                      leveldb::Slice(*it_key), &value);
    if(status.ok()){
      value_vec.push_back(value);
    }
    else{
      value_vec.push_back(NA_STRING);
    }
  }

  return value_vec;
}



// [[Rcpp::export]]
void cppDbPut(SEXP xpt,
              const std::vector<std::string> &keys,
              const std::vector<std::string> &values,
              const bool sync)
{
  Rcpp::XPtr<leveldb::DB> ptr(xpt);

  // prepare batch
  leveldb::WriteBatch batch;
  for(std::vector<std::string>::const_iterator it_key = keys.begin(), it_val = values.begin();
      it_key != keys.end(); ++it_key, ++it_val){
    batch.Put(leveldb::Slice(*it_key),
              leveldb::Slice(*it_val));
  }

  // set options
  leveldb::WriteOptions write_options;
  write_options.sync = sync;

  // write to db
  leveldb::Status status = ptr->Write(write_options, &batch);

  // TODO: check status
}

// [[Rcpp::export]]
void cppDbDelete(SEXP xpt,
              const std::vector<std::string> &keys,
              const bool sync)
{
  Rcpp::XPtr<leveldb::DB> ptr(xpt);

  // prepare batch
  leveldb::WriteBatch batch;
  for(std::vector<std::string>::const_iterator it_key = keys.begin();
      it_key != keys.end(); ++it_key){
    batch.Delete(leveldb::Slice(*it_key));
  }

  // set options
  leveldb::WriteOptions write_options;
  write_options.sync = sync;

  // write to db
  leveldb::Status status = ptr->Write(write_options, &batch);

  // TODO: check status
}


// [[Rcpp::export]]
Rcpp::List cppApply(SEXP xpt,
              Rcpp::Function fun,
              const uint num_item)
{
  Rcpp::XPtr<leveldb::DB> ptr(xpt);

  leveldb::Iterator *it = ptr->NewIterator(leveldb::ReadOptions());
  Rcpp::List result_list;
  uint count_item = 0;
  for(it->SeekToFirst(); it->Valid(); it->Next()){
    Rcpp::CharacterVector key_value = Rcpp::CharacterVector::create(
      Rcpp::Named("key") = it->key().ToString(),
      Rcpp::Named("value") = it->value().ToString());

    result_list.push_back(Rcpp::as<Rcpp::List>(fun(key_value)));

    count_item++;
    if(num_item > 0 && count_item >= num_item){
      break;
    }
  }

  delete it;

  return result_list;
}
