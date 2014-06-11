Usage Instructions
==================

- Look here for full background on this tool -- http://www.openstreetmap.org/user/dalek2point3/diary/22083

   `make`

   `wget http://osm.personalwerk.de/full-history-extracts/2014-03-31/north-america/seattle.osh.pbf`

   `./extract seattle.osh.pbf node > node.csv`

   `./extract seattle.osh.pbf way > way.csv`

- This program takes an .osh.pbf (OSM History file) as an input and spits out

  - In `node` mode: a list of nodes with either amenity, place or addr tags and metadata (name, gnis codes)
  - In `way` mode: a list of all ways with either highway, building, parking, amenity tags and metadata (tiger, access, oneway, maxspeed, lanes)