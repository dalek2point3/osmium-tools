// The code in this file is released into the Public Domain.

#include <iostream>
#include <osmium.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/ostream.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_table.hpp>

typedef osmium::index::map::Dummy<osmium::unsigned_object_id_type, osmium::Location> index_neg_type;
typedef osmium::index::map::SparseTable<osmium::unsigned_object_id_type, osmium::Location> index_pos_type;
typedef osmium::handler::NodeLocationsForWays<index_pos_type, index_neg_type> location_handler_type;

// TODO: deal with this hack
const char* na = "NA";

const char* check_null(const char* &val) {
  const char* name = val;
  // const char* na = "NA";

  if (name){
    return name;
  }
  else {
    return na;
  }
  return na;
}

struct NodeHandler : public osmium::handler::Handler {

  void node(const osmium::Node& node) {

    // filter tags
    const char* amenity = node.tags().get_value_by_key("amenity");
    const char* addr = node.tags().get_value_by_key("addr:housenumber");
    const char* place = node.tags().get_value_by_key("place");

    int flag = (check_null(amenity) != na || check_null(addr) != na || check_null(place)!=na);

    // additional tags
    const char* name = node.tags().get_value_by_key("name");
    const char* gnisfid = node.tags().get_value_by_key("gnis:feature_id");
    const char* gnisfcode = node.tags().get_value_by_key("gnis:fcode");

    if (flag!=0) {

      std::cout << check_null(amenity) << "\t" 
		<< check_null(addr) << "\t" 
		<< check_null(place) << "\t" 
		<< node.user() << "\t"
		<< node.uid() << "\t"
		<< node.timestamp() << "\t"
		<< node.version() << "\t"
		<< node.changeset() << "\t"
		<< node.location().lon() << "\t"
		<< node.location().lat() << "\t"
		<< node.id() << "\t"
		<< check_null(name) << "\t" 
		<< check_null(gnisfid) << "\t" 
		<< check_null(gnisfcode) << "\t" 
		<< std::endl;
    }
  }
};

struct WayHandler : public osmium::handler::Handler {

  void way(const osmium::Way& way) {

    // TODO: systematize this -- some "filter" keys and some "tags"
    const char* amenity = way.tags().get_value_by_key("amenity");
    const char* highway = way.tags().get_value_by_key("highway");
    const char* building = way.tags().get_value_by_key("building");
    const char* parking = way.tags().get_value_by_key("parking");

    int flag = (check_null(highway) != na || check_null(amenity) != na || check_null(building)!=na || check_null(parking) != na);

    // Other tags
    const char* name = way.tags().get_value_by_key("name");
    const char* tigercfcc = way.tags().get_value_by_key("tiger:cfcc");
    const char* tigercounty = way.tags().get_value_by_key("tiger:county");
    const char* tigerr = way.tags().get_value_by_key("tiger:reviewed");
    const char* access = way.tags().get_value_by_key("access");
    const char* oneway = way.tags().get_value_by_key("oneway");
    const char* maxspeed = way.tags().get_value_by_key("maxspeed");
    const char* lanes = way.tags().get_value_by_key("lanes");
    
    if (flag!=0) {

      // std::cerr << way.nodes().front().visible();
      double x = -1;
      double y = -1;
    
      for (const auto& wn : way.nodes()) {
	if(wn.location().lon() < 200){
	  x = wn.location().lat();
	  y = wn.location().lon();
	  break;
	}
	// std::cerr << "getting another node " << y << "\n";
      }

      std::cout	<< check_null(highway) << "\t" 
		<< check_null(amenity) << "\t" 
		<< check_null(building) << "\t" 
		<< check_null(parking) << "\t" 
		<< way.user() << "\t"
		<< way.uid() << "\t"
		<< way.timestamp() << "\t"
		<< way.version() << "\t"
		<< way.changeset() << "\t"
		<< x << "\t"
		<< y << "\t"
		<< way.id() << "\t"
		<< check_null(name) << "\t" 
		<< check_null(tigercfcc) << "\t" 
		<< check_null(tigercounty) << "\t" 
		<< check_null(tigerr) << "\t" 
		<< check_null(access) << "\t" 
		<< check_null(oneway) << "\t" 
		<< check_null(maxspeed) << "\t" 
		<< check_null(lanes) << "\t" 
		<< std::endl;
    }
  }  
};


int main(int argc, char* argv[]) {
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " OSMFILE FEATURETYPE\n";
      exit(1);
    }

    index_pos_type index_pos;
    index_neg_type index_neg;
    location_handler_type location_handler(index_pos, index_neg);
    location_handler.ignore_errors();

    if(strcmp ("node",argv[2]) == 0){
      NodeHandler node_handler;
      std::cerr << "processing node\n";
      osmium::io::Reader reader(argv[1], osmium::osm_entity::flags::node);
      osmium::apply(reader, location_handler, node_handler);
    }

    if(strcmp ("way",argv[2]) == 0){
      WayHandler way_handler;
      std::cerr << "processing way\n";
      osmium::io::Reader reader(argv[1], osmium::osm_entity::flags::all);
      osmium::apply(reader, location_handler, way_handler);
    }

}

