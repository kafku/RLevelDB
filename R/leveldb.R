
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
OpenLevelDB <- function(path, create = FALSE){
  if(!create && !file.exists(path)){
    stop(paste0("cannot find ", path))
  }

  db.ptr <- cppOpenLevelDB(path, create)

  return(new("LevelDB", path = path, ptr = db.ptr))
}
