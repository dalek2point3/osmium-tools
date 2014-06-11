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

struct NodeHandler : public osmium::handler::Handler {

  void node(const osmium::Node& node) {
    const char* amenity = node.tags().get_value_by_key("amenity");
    if (amenity && amenity[0] != '\0') {
      const char* name = node.tags().get_value_by_key("name");
      const char* amenity = node.tags().get_value_by_key("amenity");
      if (name) {
  	std::cout << name << "\t" 
  		  << amenity << "\t" 
  		  << node.user() << "\t"
  		  << node.uid() << "\t"
  		  << node.timestamp() << "\t"
  		  << node.version() << "\t"
  		  << node.changeset() << "\t"
  		  << node.location().lon() << "\t"
  		  << node.location().lat() << "\t"
  		  << node.id()
  		  << std::endl;
      }
    }
  }
};

struct WayHandler : public osmium::handler::Handler {

  void way(const osmium::Way& way) {
   
    const char* highway = way.tags().get_value_by_key("highway");
    const char* name = way.tags().get_value_by_key("name");
    
    if (highway && highway[0] != '\0') {

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

      if (name){
	std::cout << name << "\t";
      }
      else {
	std::cout << "NA" << "\t";
      }

      std::cout	<< highway << "\t" 
		<< way.user() << "\t"
		<< way.uid() << "\t"
		<< way.timestamp() << "\t"
		<< way.version() << "\t"
		<< way.changeset() << "\t"
		<< x << "\t"
		<< y << "\t"
		<< way.id()
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

