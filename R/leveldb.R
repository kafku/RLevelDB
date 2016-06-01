
setClass(
  Class = "LevelDB",
  representation = representation(path = "character", ptr = "externalptr"))


open.leveldb <- function(path, create = FALSE){
  if(!create && !file.exists(path)){
    stop(paste0("cannot find ", path))
  }

  db.ptr <- cppOpenLevelDB(path, create)

  return(new("LevelDB", path, db.ptr))
}
