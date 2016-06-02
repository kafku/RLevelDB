# RLevelDB

R interface for [LevelDB](https://github.com/google/leveldb).

## Examples

```R
db <- OpenLevelDB(path = "./testdb", create = T)

dbPut(db, keys = c("key1", "key2", "key3"), values = c("A", "B", "C"), sync = F)
dbGet(db, keys = c("key1", "key3")) # => c("A", "C")
dbDelete(db, keys = "key2", sync = F)
dbGet(db, keys = c("key1", "key2", "key3")) # => c("A", NA, "C")
```

## License

GPL (>= 2)

