/*
 * test.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: santiago
 */
#include<Mocho/Lua/lua.hpp>
#include<Mocho/Graphics/SpriteAnimation.hpp>
#include<Mocho/Graphics/SpriteBatch.hpp>
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
	SpriteAnimation s0(&texture, frameSize);
	s0.setFrameSpeed(sf::seconds(1.f),8);
	s0.setAnimationType(SpriteAnimation::Type::Once);
	s0.getSprite().setScale(0.5f,0.5f);

	SpriteAnimation s1(&texture, frameSize);
	s1.setFrameSpeed(sf::seconds(1.f),8);
	s1.setAnimationType(SpriteAnimation::Type::Loop);
	s1.getSprite().setPosition(Vec2f(0,frameSize.y*0.5f));
	s1.getSprite().setScale(0.5f,0.5f);

	SpriteAnimation s2(&texture, frameSize);
	s2.setFrameSpeed(sf::seconds(1.f),8);
	s2.setAnimationType(SpriteAnimation::Type::BackAndForth);
	s2.getSprite().setPosition(Vec2f(0,frameSize.y));
	s2.getSprite().setScale(0.5f,0.5f);

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
		s0.update(dt);
		s1.update(dt);
		s2.update(dt);
		win.clear();
		win.draw(s0.getSprite());
		win.draw(s1.getSprite());
		win.draw(s2.getSprite());
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
