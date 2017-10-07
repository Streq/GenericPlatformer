/*
 * LoadingState.hpp
 *
 *  Created on: Sep 30, 2017
 *      Author: santiago
 */

#pragma once

#include <Mocho/Application/AppState.hpp>

namespace mch {

class LoadingState: public AppState {
	public:

		//Push an actual gameState lmao
		virtual bool update();

};

} /* namespace mch */
