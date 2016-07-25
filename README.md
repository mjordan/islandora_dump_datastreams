# Islandora Dump Datastreams

Utility module that writes out an object's datastreams to the specified directory upon ingestion. The typical use case for this module is to generate derivates for ingestion into another Islandora instance using the [Islandora Batch with Derivs](https://github.com/mjordan/islandora_batch_with_derivs) module. But, its output could be used for other purposes, such as backup, migration, or preservation purposes.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Usage

Enable this module, and configure it at admin/islandora/tools/dump_datastreams. Options include the output directory and a list of datastream IDs that you do not want dumped.

It is extremely important that you are aware when this module is enabled, since it makes a copy of every object you ingest into your Islandora.

### Example output

Each object that is ingested will get its own subdirectory within the output directory. Within each object directory will be written all the files that correspond to the object's datastreams, named using datastream IDs and extensions that map to the datastream's MimeType:

```
/tmp/ouputdirectory
├── islandora_23 
│   ├── DC.xml
│   ├── MEDIUM_SIZE.jpg
│   ├── MODS.xml
│   ├── OBJ.jpg
│   ├── TECHMD.xml
│   └── TN.jpg
├── islandora_24
│   ├── DC.xml
│   ├── MEDIUM_SIZE.jpg
│   ├── MODS.xml
│   ├── OBJ.jpg
│   ├── TECHMD.xml
│   └── TN.jpg
└── islandora_25
    ├── DC.xml
    ├── MEDIUM_SIZE.jpg
    ├── MODS.xml
    ├── OBJ.jpg
    ├── TECHMD.xml
    └── TN.jpg
```

The names of the object subdirectories are derived from the object's PID.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

 [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
