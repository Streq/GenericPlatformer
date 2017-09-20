/*
 * test.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: santiago
 */
#include<Mocho/lua.hpp>
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



}
