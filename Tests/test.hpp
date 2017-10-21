/*
 * test.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: santiago
 */
#include<Mocho/Lua/lua.hpp>
#include<Mocho/Graphics.hpp>
namespace mch{
const char* hola(int a, int b){
	return "a+b";
}

void test(){
	lua::Script scr;
	//scr.doBuffer("function add(a, b) return a + b end");
	scr.doFile("vars.lua");
	int a = 0;
	lua::Script::function<int,int>::call<int>(scr,"mul", 10, 5, a);
	printf("%d\n",a);


	scr.registerFunction("hola",lua::Script::cfunction<int,int>::ret<const char*>::function<hola>);
	const char* c;
	lua::Script::function<int,int>::call<const char*>(scr,"hola",10, 2, c);
	printf("%s\n",c);
}

void testSpriteAnimation(){
	sf::Texture texture;
	texture.loadFromFile("runningcat.png");
	auto bounds = texture.getSize();
	auto frameSize = Vec2u(bounds.x/2u,bounds.y/4u);
	SpriteSheetBuilder ssb;
	ssb.setSize(frameSize);

	SpriteGallery onceGallery;
	onceGallery.setAnimationType(mch::SpriteGallery::Type::Once);
	onceGallery.setSprites(ssb.makeSheet(texture));

	SpriteGallery loopGallery;
	loopGallery.setAnimationType(mch::SpriteGallery::Type::Loop);
	loopGallery.setSprites(ssb.makeSheet(texture));

	SpriteGallery bafGallery;
	bafGallery.setAnimationType(mch::SpriteGallery::Type::BackAndForth);
	bafGallery.setSprites(ssb.makeSheet(texture));

	SpriteAnimation a0;
	a0.setSpriteGallery(onceGallery);
	a0.setFrameTime(sf::seconds(1.f/8.f));

	SpriteAnimation a1;
	a1.setSpriteGallery(loopGallery);
	a1.setFrameTime(sf::seconds(1.f/8.f));

	SpriteAnimation a2;
	a2.setSpriteGallery(bafGallery);
	a2.setFrameTime(sf::seconds(1.f/8.f));

	sf::RenderWindow win(sf::VideoMode(800,600),"prueba animacion sprite");
	sf::Clock clock;
	while(win.isOpen()){
		sf::Event e;
		while(win.pollEvent(e)){
			if(e.type==sf::Event::Closed){
				win.close();
			}
		}
		auto dt = clock.restart();
		sf::Transform t0;
		sf::Transform t1;
		sf::Transform t2;
		t0.scale(0.5f,0.5f);
		t1.scale(0.5f,0.5f);
		t2.scale(0.5f,0.5f);
		t0.translate(0.f,0.f);
		t1.translate(0.f,300.f);
		t2.translate(0.f,600.f);

		a0.update(dt);
		a1.update(dt);
		a2.update(dt);
		win.clear();
		win.draw(a0.getCurrentFrame(),t0);
		win.draw(a1.getCurrentFrame(),t1);
		win.draw(a2.getCurrentFrame(),t2);
		win.display();
	}


}

void testSpriteBatch(){
	sf::RenderWindow win(sf::VideoMode(800,600),"juas");

	sf::Transform trans;

	int i=0;

	sf::Texture text;
	text.loadFromFile("runningcat.png");
	Vec2u frame = Vec2u(mch::vec::scale
			(mch::Vec2f(text.getSize())
			,mch::Vec2f(0.5f,0.25f)));

	while(true){
		++i;
		sf::Event e;
		win.pollEvent(e);
		if(e.type==sf::Event::Closed)break;

		sf::Sprite s;
		s.setTextureRect(sf::IntRect(0.f,0.f,frame.x,frame.y));
		s.setTexture(text,false);

		SpriteBatch batch(text,3);
		s.setPosition(300.f,300.f);
		auto bounds = s.getLocalBounds();
		s.setOrigin(bounds.width*0.5f,bounds.height*0.5f);
		//s.rotate(5+0.1*++i);
		int j = int(i*0.01);
		s.setTextureRect(sf::IntRect((j%2)*frame.x,((j/2)%4)*frame.y,frame.x,frame.y));
		batch.addSprite(s);

		win.clear();
		//win.draw(vert,sizeVert,sf::PrimitiveType::TrianglesStrip);
		win.draw(batch);
		win.display();
	};

}


}
