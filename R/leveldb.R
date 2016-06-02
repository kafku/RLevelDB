
#' @export
setClass(
  Class = "LevelDB",
  slots = list(path = "character", ptr = "externalptr"))


#' @export
OpenLevelDB <- function(path, create = FALSE){
  if(!create && !file.exists(path)){
    stop(paste0("cannot find ", path))
  }

  db.ptr <- cppOpenLevelDB(path, create)

  return(new("LevelDB", path = path, ptr = db.ptr))
}
