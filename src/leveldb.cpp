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

  // prepare batch
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

