#include "TMXMap.h"
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <TextureManager.h>
#include <sstream>
#include <Sprite.h>
#include <RigidBody.h>
#include <Camera.h>
#include <LuaScript.h>
#include <ParticleSystem.h>
#include <LightManager.h>

static void TMXCheckProperties(Bundle *properties, rapidxml::xml_node<> *node);

TMXMap* TMXMap::create(const std::string &name) {
	TMXMap* map = new TMXMap();
	map->setName(name);
	return map;
}

const unsigned int& TMXMap::getTileWidth() const {
	return m_tileWidth;
}

const unsigned int& TMXMap::getTileHeight() const {
	return m_tileHeight;
}

void TMXMap::onCreate() {
	load("data/"+getName()+".tmx");
	Node::onCreate();
}

void TMXMap::onPreRender() {
	glClearColor(getColor().r/255.0f, getColor().g/255.0f, getColor().b/255.0f, 1);
	Node::onPreRender();
}

void TMXMap::onDestroy() {
	delete tileSet;
	Node::onDestroy();
}

void TMXMap::load(const std::string &filename) {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root, *node;

	rapidxml::file<> f(filename.c_str());
	doc.parse<0>(f.data());

	root = doc.first_node("map");
	char *attr;

	int gridWidth = std::atoi(root->first_attribute("width")->value());
	int gridHeight = std::atoi(root->first_attribute("height")->value());
	m_tileWidth = std::atoi(root->first_attribute("tilewidth")->value());
	m_tileHeight = std::atoi(root->first_attribute("tileheight")->value());
	setSize(gridWidth*m_tileWidth, gridHeight*m_tileHeight);
	
	Color backgroundColor;
	if(root->first_attribute("backgroundcolor")) {
		backgroundColor = Color(root->first_attribute("backgroundcolor")->value());
	}
	else {
		backgroundColor = Color::fromFloat(0.0f, 0.0f, 0.0f, 1.0f);
	}
	setColor(backgroundColor);

	TMXCheckProperties(&getProperties(), root);
	if(getProperties().contains("ambientLight")) {
		Color color = std::any_cast<Color>(getProperties()["ambientLight"]);
		LightManager::instance()->setAmbientLight(color);
	}

	node = root->first_node("tileset");
	attr = node->first_attribute("source")->value();
	std::string tileSetFilename = filename.substr(0, filename.find_last_of("/")) + "/" + attr;
	loadTileSet(tileSetFilename);

	node = root->first_node();
	int z = 0;
	while(node) {
		std::string layerName = node->name();
		bool visible = !node->first_attribute("visible") || (std::atoi(node->first_attribute("visible")->value()));

		if(layerName == "layer") {
			std::string name = node->first_attribute("name")->value();
			int width = std::atoi(node->first_attribute("width")->value());
			int height = std::atoi(node->first_attribute("height")->value());
			rapidxml::xml_node<> *child = node->first_node("data");

			attr = child->value();
			std::istringstream stream(attr);
			std::string token;
			unsigned int i = 0;
			std::vector<TMXTileLayerData> data;
			while(std::getline(stream, token, ',')) {
				int tile = std::atoi(token.c_str()) - 1;
				if(tile >= 0) {
					TMXTileLayerData d;
					d.x = (i%gridWidth*m_tileWidth - getWidth()/2.0f);
					d.y = (getHeight() - i/gridHeight*m_tileHeight - getHeight()/2.0f);
					d.tile = tile;
					data.push_back(d);
				}
				i++;
			}
			auto layer = new TMXTileLayer(name, getWidth(), getHeight(), tileSet, data);
			layer->setZOrder(z);
			addNode(layer);
		}
		else if(layerName == "objectgroup") {
			rapidxml::xml_node<> *child;
			child = node->first_node("object");
			while(child) {
				std::string name = child->first_attribute("name")->value();
				std::string type = child->first_attribute("type") ? child->first_attribute("type")->value() : "UNKNOWN";
				float x, y, width, height;
				if(child->first_attribute("x")) {
					x = std::atof(child->first_attribute("x")->value());
				}
				if(child->first_attribute("y")) {
					y = std::atof(child->first_attribute("y")->value());
				}
				if(child->first_attribute("width")) {
					width = std::atof(child->first_attribute("width")->value());
				}
				if(child->first_attribute("height")) {
					height = std::atof(child->first_attribute("height")->value());
				}
				x = (x + width/2.0f - getWidth()/2.0f)/m_tileWidth;
				y = (getHeight()-y - height/2.0f - getHeight()/2.0f)/m_tileHeight;
				width /= m_tileWidth;
				height /= m_tileHeight;

				Bundle properties;
				TMXCheckProperties(&properties, child);
				if(type == "DirectionalLight") {
					Color color = std::any_cast<Color>(properties["color"]);
					float angle = properties.get<float>("angle", 90.0f);
					int rays = properties.get<int>("rays", 4);
					LightManager::instance()->createDirectionalLight(rays, color, angle);
				}
				else if(type == "PointLight") {
					Color color = std::any_cast<Color>(properties["color"]);
					float distance = properties.get<float>("distance", 10);
					int rays = properties.get<int>("rays", 4);
					LightManager::instance()->createPointLight(rays, color, distance, x, y);
				}
				else {
					Node* obj;
					if(type == "Sprite") {
						obj = Sprite::create(name);
						std::string textureFile = std::any_cast<std::string>(properties["sprite"]);
						auto texture = TextureManager::instance()->load("graphics/" + textureFile);
						if(properties.contains("row") && properties.contains("col")) {
							int row = std::any_cast<int>(properties["row"]);
							int col = std::any_cast<int>(properties["col"]);
							int sid = std::any_cast<int>(properties["sid"]);
							auto sprites = TextureRegion::split(texture, row, col, 24);
							for(TextureRegion& sprite : sprites) {
								sprite.offsetY = 6;
							}
							((Sprite*)obj)->setTexture(sprites[sid]);
						}
						else {
							((Sprite*)obj)->setTexture(texture);
						}
					}
					else if(type == "Camera") {
						obj = new Camera();
						obj->setPosition(x, y);
						((Camera*)obj)->setViewport(width, height);
						((Camera*)obj)->setZoom(1.0f);
					}
					else if(type == "Particle") {
						obj = ParticleSystem::create(name);
						obj->setPosition(x, y);
						obj->setSize(width, height);
					}
					else {
						obj = Node::create(name);
					}
					obj->setZOrder(z);
					obj->setProperties(properties);

					if(properties.contains("static")) {
						bool isStatic = properties.get("static", false);
						b2BodyDef def;
						def.fixedRotation = true;
						def.type = isStatic ? b2_staticBody : b2_dynamicBody;
						def.position.Set(x, y);
						b2PolygonShape boxShape;
						boxShape.SetAsBox(width/2.0f, height/2.0f);

						b2FixtureDef fixtureDef;
						fixtureDef.shape = &boxShape;

						RigidBody *body = new RigidBody(&def);
						body->createFixture(&fixtureDef);
						obj->addComponent(body);

						if(!isStatic) {
							boxShape.SetAsBox(width*0.4f, height*0.05f, b2Vec2(0.0f, -height/2.0f), 0);
							fixtureDef.isSensor = true;
							b2Fixture *fixture = body->createFixture(&fixtureDef);
							fixture->SetUserData(body->sensors + SensorType::FOOT);

							boxShape.SetAsBox(width*0.4f, height*0.05f, b2Vec2(0.0f, height/2.0f), 0);
							fixtureDef.isSensor = true;
							fixture = body->createFixture(&fixtureDef);
							fixture->SetUserData(body->sensors + SensorType::HEADER);

							boxShape.SetAsBox(width*0.05f, height*0.4f, b2Vec2(width/2.0f, 0.0f), 0);
							fixtureDef.isSensor = true;
							fixture = body->createFixture(&fixtureDef);
							fixture->SetUserData(body->sensors + SensorType::FRONT);

							boxShape.SetAsBox(width*0.05f, height*0.4f, b2Vec2(-width/2.0f, 0.0f), 0);
							fixtureDef.isSensor = true;
							fixture = body->createFixture(&fixtureDef);
							fixture->SetUserData(body->sensors + SensorType::BACK);
						}
					}

					if(properties.contains("behavior")) {
						std::string luaName = std::any_cast<std::string>(properties["behavior"]);
						obj->addComponent(new LuaScript(luaName));
					}
					
					addNode(obj);
				}

				child = child->next_sibling();
			}
		}
		z++;
		node = node->next_sibling();
	}
}

void TMXMap::loadTileSet(const std::string &filename) {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root, *node;

	rapidxml::file<> f(filename.c_str());
	doc.parse<0>(f.data());

	root = doc.first_node("tileset");
	tileSet = new TMXTileSet(root->first_attribute("name")->value());

	std::string textureFile = root->first_node("image")->first_attribute("source")->value();
	textureFile = "graphics/" + textureFile.substr(textureFile.find_last_of("/")+1);
	auto texture = TextureManager::instance()->load(textureFile);
	tileSet->tileWidth = std::atoi(root->first_attribute("tilewidth")->value());
	tileSet->tileHeight = std::atoi(root->first_attribute("tileheight")->value());
	tileSet->tileCount = std::atoi(root->first_attribute("tilecount")->value());
	tileSet->columns = std::atoi(root->first_attribute("columns")->value());
	int row = texture->height()/tileSet->tileHeight;
	int col = texture->width()/tileSet->tileWidth;
	tileSet->sprites = TextureRegion::split(texture, row, col, tileSet->tileWidth);

	node = root->first_node("tile");
	while(node) {
		int tile = std::atoi(node->first_attribute("id")->value());
		TMXTile *tileObject = new TMXTile();
		TMXCheckProperties(&tileObject->getProperties(), node);
		rapidxml::xml_node<> *frame;
		if(node->first_node("animation")) {
			std::vector<int> frames;
			float duration = 0;
			frame = node->first_node("animation")->first_node("frame");
			while(frame) {
				int tileid = std::atoi(frame->first_attribute("tileid")->value());
				int frameDuration = std::atoi(frame->first_attribute("duration")->value());
				frames.push_back(tileid);
				duration += frameDuration / 1000.0f;
				frame = frame->next_sibling();
			}
			tileObject->animation = new Animation<int>(duration, AnimationMode::LOOP, frames);
			tileSet->tiles[tile] = tileObject;
		}
		node = node->next_sibling();
	}
}

void TMXCheckProperties(Bundle *properties, rapidxml::xml_node<> *node) {
	if(node->first_node("properties")) {
		rapidxml::xml_node<> *property = node->first_node("properties")->first_node("property");
		while(property) {
			std::string name = property->first_attribute("name")->value();
			std::string type = property->first_attribute("type") ? property->first_attribute("type")->value() : "string";
			char* attr = property->first_attribute("value")->value();
			size_t size;
			if(type == "bool") {
				bool value = strcmp(attr, "true") == 0;
				(*properties)[name] = value;
			}
			else if(type == "color") {
				Color c(attr); // skip #
				(*properties)[name] = c;
			}
			else if(type == "float") {
				float v = std::atof(attr);
				(*properties)[name] = v;
			}
			else if(type == "int") {
				int v = std::atoi(attr);
				(*properties)[name] = v;
			}
			else if(type == "file") {
				std::string f = attr;
				int p = f.find_last_of("/")+1;
				attr = attr + p;
				(*properties)[name] = std::string(attr);
			}
			else {
				(*properties)[name] = std::string(attr);
			}

			property = property->next_sibling();
		}
	}
}