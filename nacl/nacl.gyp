{
  "includes": [
    "../gypi/global.gypi"
  ],
  "target_defaults": {
    "type": "executable",
    "ldflags": ["-pthread"],
    "libraries": ["-lppapi", "-lppapi_cpp", "-ljsoncpp"],
    "sources": [
      "opencc.cc",
      "../src/config_reader.c",
      "../src/converter.c",
      "../src/dictionary_group.c",
      "../src/dictionary_set.c",
      "../src/encoding.c",
      "../src/utils.c",
      "../src/opencc.c",
      "../src/dictionary/abstract.c",
      "../src/dictionary/datrie.c",
      "../src/dictionary/text.c"
    ]
  },
  "targets": [{
    "target_name": "opencc_x86_32.nexe",
    "cflags": ["-m32"],
    "ldflags": ["-m32"]
  }, {
    "target_name": "opencc_x86_64.nexe",
    "cflags": ["-m64"],
    "ldflags": ["-m64"]
  }]
}
