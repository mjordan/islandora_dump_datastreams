# Islandora Dump Datastreams

Utility module that writes out an object's datastreams to the specified directory upon ingestion. The original use case for this module was to generate derivates for ingestion into another Islandora instance using the [Islandora Batch with Derivs](https://github.com/mjordan/islandora_batch_with_derivs) module. But, its output could be used for other purposes, such as backup, migration, or preservation storage.

Currently, this module dumps datastreams for:

* single-file objects (e.g. basic image, large image, PDF, video, audio, etc.)
* books
* newspaper issues

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Usage

Enable this module, and configure it at admin/islandora/tools/dump_datastreams. This module has a kill switch ("Enable dumping of objects and their datastreams.") and only dumps out files if that box is checked.

> It is extremely important that you are aware when this module is enabled and active, since it makes a copy of every object you ingest into your Islandora.

The other configuration options - output directory, DSIDs to exclude, and content model - are pretty self-explanatory.

### Example output

Each object that is ingested will get its own subdirectory beneath the output directory. Within each object directory will be written all the files that correspond to the object's datastreams (excluding datastreams that you configure the module to "skip"), named using datastream IDs and extensions that map to the datastream's MimeType:

```
/tmp/ouputdirectory
├── islandora_23 
│   ├── MEDIUM_SIZE.jpg
│   ├── MODS.xml
│   ├── OBJ.jpg
│   ├── TECHMD.xml
│   └── TN.jpg
├── islandora_24
│   ├── MEDIUM_SIZE.jpg
│   ├── MODS.xml
│   ├── OBJ.jpg
│   ├── TECHMD.xml
│   └── TN.jpg
└── islandora_25
    ├── MEDIUM_SIZE.jpg
    ├── MODS.xml
    ├── OBJ.jpg
    ├── TECHMD.xml
    └── TN.jpg
```

The names of the object subdirectories are derived from the source object's PID so they are guaranteed to be unique. The output produced for books and newspaper issues is consistent with the input documented in the READMEs for the [Islandora Book Batch](https://github.com/Islandora/islandora_book_batch) and [Islandora Newspaper Batch](https://github.com/Islandora/islandora_newspaper_batch) modules.

## Usage in conjuction with the Islandora Batch with Derivs (and Islandora Book Batch and Islandora Newspaper Batch) modules

As stated above, this module was originally intended to generate input for the [Islandora Batch with Derivs](https://github.com/mjordan/islandora_batch_with_derivs) module as part of a strategy for speeding up large ingests. This strategy takes advantage of the idea that having pregenerated datastreams allows you to enable Islandora's "Defer derivative generation during ingest" option, which prevents all derivative-creation code from being fired, increasing ingest throughput substantially.

One possible scenario is to set up a number of "worker" Islandora instances (using [Islandora Vagrant](https://github.com/Islandora-Labs/islandora_vagrant), for example), and then to ingest content into them using the standard Islandora Batch modules, [Batch](https://github.com/Islandora/islandora_batch), [Book Batch](https://github.com/Islandora/islandora_book_batch), and [Newspaper Batch](https://github.com/Islandora/islandora_newspaper_batch). Enabling Islandora Dump Datastreams on the worker Islandoras will result in a set of pregenerated datastreams that can then be ingested into the production Islandora quickly using Islandora Batch with Derivs. This strategy can be scaled up easily to massively parallelize the generation of derivatives.

![Using worker instances of Islandora to pregenerate derivatives for ingestion](https://dl.dropboxusercontent.com/u/1015702/linked_to/Islandora%20Dump%20Datastreams%20-%20Using%20worker%20Islandora%20instances%20to%20generate%20derivatives%20for%20ingest.png)

PIDs, and relationships between objects, are not guaranteed to be portable across Islandora instances. Consequently, PIDs generated for objects in the worker Islandora instances should be considered "temporary" since the objects will get new PIDs when they are reingested into the production Islandora instance. Some datastreams, in particular RELS-EXT, RELS-INT, and DC, contain source objects' PIDs and relationship information that will conflict or override the equivalent data generated on reingest. If you are using this module in the scenario described here, you will want to exlude those datastreams from this module's output.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## Further Development

* Add logic to output generic compound objects and their childrens' derivatives
* Add the ability to purge an object after it has been dumped, so that worker Islandora instances do not not need to keep two "copies" of each object (the ingested copy and the dumped copy)

## License

 [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
