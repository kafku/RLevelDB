
#' @export
setClass(
  Class = "LevelDB",
  slots = list(path = "character", ptr = "externalptr"))

#' @export
#' @export
setGeneric(
  name = "dbGet",
  def = function(db, keys){
    standardGeneric('dbGet')
  })

setMethod(
  f = "dbGet",
  signature = signature(db = "LevelDB", keys = "character"),
  definition = function(db, keys){
    return(cppDbGet(db@ptr, keys))
  })

#' @export
setGeneric(
  name = "dbPut",
  def = function(db, keys, values, sync = FALSE){
    standardGeneric('dbPut')
  })

put.leveldb <- function(db, keys, values, sync = FALSE){
  # TODO: check length and dim

  # convert values to string
  non.character <- !is.character(values)
  non.character <- !is.character(values)
  if(any(non.character)){
    values[non.character] <- as.character(values[non.character])
  }

  cppDbPut(db@ptr, keys, values, sync)
}

setMethod(
  f = "dbPut",
  signature = signature(db = "LevelDB", keys = "character",
                        values = "ANY", sync = "logical"),
  definition = function(db, keys, values, sync){
    put.leveldb(db, keys, values, sync)
  })


setMethod(
  f = "dbPut",
  signature = signature(db = "LevelDB", keys = "character",
                        values = "ANY", sync = "missing"),
  definition = function(db, keys, values){
    put.leveldb(db, keys, values)
  })


#' @export
setGeneric(
  name = "dbDelete",
  def = function(db, keys, sync = FALSE){
    standardGeneric('dbDelete')
  })

delete.leveldb <- function(db, keys, sync = FALSE){
  cppDbDelete(db@ptr, keys, sync)
}

setMethod(
  f = "dbDelete",
  signature = signature(db = "LevelDB", keys = "character", sync = "logical"),
  definition = function(db, keys, sync){
    delete.leveldb(db, keys, sync)
  })


setMethod(
  f = "dbDelete",
  signature = signature(db = "LevelDB", keys = "character", sync = "missing"),
  definition = function(db, keys){
    delete.leveldb(db, keys)
  })

#' @export
setGeneric(
  name = "dbApply",
  def = function(db, FUN, n = 0, ...){
    standardGeneric("dbApply")
  })

setMethod(
  f = "dbApply",
  signature = signature(db = "LevelDB", FUN = "function", n = "numeric"),
  definition = function(db, FUN, n, ...){
    FUN <- match.fun(FUN)
    if(n < 0) {
      stop("n must be non negative.")
    }

    return(cppApply(db@ptr, FUN, n))
  })

#' @export
OpenLevelDB <- function(path, create = FALSE){
  if(!create && !file.exists(path)){
    stop(paste0("cannot find ", path))
  }

  db.ptr <- cppOpenLevelDB(path, create)

  return(new("LevelDB", path = path, ptr = db.ptr))
}
