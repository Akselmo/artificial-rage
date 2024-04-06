import json
import sys
from typing import Any

# Run command:
# python ./generate.py ./entities.json ./map_template.json

class MapConfigGenerator():
    # Data loaded from json
    entities_data: dict[str, Any] = {}
    map_data: dict[str, Any] = {}
    #  Loaded values
    entities: dict[int, str] = {}     #entities.json/tiles[first].{id, type}
    entities_count: int = 0           #entities.json/tilecount
    firstgid: int = 0                 #map.json/tilesets.firstgid
    level_array: list[int] = []       #map.json/layers.[layer where name is Map].data
    level_height: int = 0             #map.json/layers.[layer where name is Map].height
    level_width: int = 0              #map.json/layers.[layer where name is Map].width
    level_name: str = ""              #map.json/properties.level_name
    level_floor_texture: str = ""     #map.json/properties.floor_texture
    level_ceiling_texture: str = ""   #map.json/properties.ceiling_texture
    level_ceiling_height: int = 1     #map.json/properties.ceiling_height

    def __init__(self, entities_file, input_map_file):
        # Read entities data
        with open(entities_file, 'r') as ef:
            self.entities_data = json.load(ef)
        # Read map data
        with open(input_map_file, 'r') as imf:
            self.map_data = json.load(imf)

    def prepare_values(self):
        # Get firstgid
        for tileset in self.map_data["tilesets"]:
            if tileset["source"] == "entities.json":
                self.firstgid = tileset["firstgid"]

        # Get all entities
        for entity in self.entities_data["tiles"]:
            self.entities[entity["id"]+self.firstgid] = entity["type"]

        self.entities_count = len(self.entities)

        # Get data from right layer
        map_layer: dict[int, str] = {}
        for layer in self.map_data["layers"]:
            if layer["name"] == "Map":
                map_layer = layer

        # Since in tiled 0 is "untouched" and in game its "empty",
        # just make any empty zones and untouched ones 0
        data_array: list[str] = []
        for item in map_layer["data"]:
            if (item <= 0):
                data_array.append(0)
            else:
                data_array.append(item - self.firstgid)
        self.level_array = str(data_array).strip(r"[]").replace(" ", "")
        self.level_height = map_layer["height"]
        self.level_width = map_layer["width"]

        # Get properties
        for prop in self.map_data["properties"]:
            if prop["name"] == "level_name":
                self.level_name = str(prop["value"]).replace(",. ", "")
            elif prop["name"] == "ceiling_height":
                self.level_ceiling_height = prop["value"]
            elif prop["name"] == "ceiling_texture":
                self.level_ceiling_texture = prop["value"]
            elif prop["name"] == "floor_texture":
                self.level_floor_texture = prop["value"]
        print(self.level_name)


    def generate_level_config(self):
        # Set map output file name
        # NOTE: This will override any similarly named level!
        level_file = f"{self.level_name}.cfg"
        with open(level_file, 'w+') as f:
            f.write(f"name={self.level_name}\n")
            f.write(f"height={self.level_height}\n")
            f.write(f"width={self.level_width}\n")
            f.write(f"data={self.level_array}\n")
            f.write(f"ceilingheight={self.level_ceiling_height}\n")
            f.write(f"floortexture={self.level_floor_texture}\n")
            f.write(f"ceilingtexture={self.level_ceiling_texture}\n")
            f.write(f"entitiestotal={self.entities_count}\n")
            for entity in self.entities:
                f.write(f"{entity}={self.entities[entity]}\n")

    def run(self):
        self.prepare_values()
        self.generate_level_config()



if __name__ == "__main__":
    mcg = MapConfigGenerator(sys.argv[1], sys.argv[2])
    mcg.run()
