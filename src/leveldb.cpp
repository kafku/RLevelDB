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


