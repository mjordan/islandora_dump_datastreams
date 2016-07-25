# Islandora Dump Datastreams

Utility module that writes out an object's datastreams to the specified directory upon ingestion. The original use case for this module was to generate derivates for ingestion into another Islandora instance using the [Islandora Batch with Derivs](https://github.com/mjordan/islandora_batch_with_derivs) module. But, its output could be used for other purposes, such as backup, migration, or preservation purposes.

Currently, this module only dumps datastreams for objects that do not have any children (e.g. basic image, large image, PDF, video, audio, etc.).

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Usage

Enable this module, and configure it at admin/islandora/tools/dump_datastreams. Options include the output directory and a list of datastream IDs that you do not want dumped.

> It is extremely important that you are aware when this module is enabled, since it makes a copy of every object you ingest into your Islandora.

### Example output

Each object that is ingested will get its own subdirectory beneath the output directory. Within each object directory will be written all the files that correspond to the object's datastreams, named using datastream IDs and extensions that map to the datastream's MimeType:

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

The names of the object subdirectories are derived from the object's PID so they are guaranteed to be unique.

## Usage in conjuction with the Islandora Batch with Derivs module

As stated above, this module was originally intended to generate input for the [Islandora Batch with Derivs](https://github.com/mjordan/islandora_batch_with_derivs) module as part of a strategy for speeding up large ingests. This strategy takes advantage of the idea that having pregenerated datastreams allows you to enable Islandora's "Defer derivative generation during ingest" option, which prevents all derivative-creation code from being fired, and which increases ingest throughput substantially.

One possible scenario is to set up a number of "worker" Islandora instances (using [Islandora Vagrant](https://github.com/Islandora-Labs/islandora_vagrant), for example), and then to ingest content into them using the standard Islandora Batch modules, [Batch](https://github.com/Islandora/islandora_batch), [Book Batch](https://github.com/Islandora/islandora_book_batch), and [Newspaper Batch](https://github.com/Islandora/islandora_newspaper_batch). Enabling Islandora Dump Datastreams on the worker Islandoras will result in a set of pregenerated datastreams that can then be ingested into the production Islandora quickly using Islandora Batch with Derivs. This strategy can be scaled up to massively parallelize the generation of derivatives.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## Further Development

* Add logic to output newspaper and book objects and all their pages' derivatives so that they can be ingested with [Book Batch](https://github.com/Islandora/islandora_book_batch), and [Newspaper Batch](https://github.com/Islandora/islandora_newspaper_batch)
* Add logic to output generic compound objects and their childrens' derivatives

## License

 [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
