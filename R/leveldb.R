
#' @export
setClass(
  Class = "LevelDB",
  slots = list(path = "character", ptr = "externalptr"))

setGeneric(
  name = "dbGet",
  def = function(db, keys){
    standardGeneric('dbGet')
  })

#' @export
setMethod(
  f = "dbGet",
  signature = signature(db = "LevelDB", keys = "character"),
  definition = function(db, keys){
    return(cppDbGet(db@ptr, keys))
  })

#' @export
OpenLevelDB <- function(path, create = FALSE){
  if(!create && !file.exists(path)){
    stop(paste0("cannot find ", path))
  }

  db.ptr <- cppOpenLevelDB(path, create)

  return(new("LevelDB", path = path, ptr = db.ptr))
}
