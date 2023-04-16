// register state on-demand
template <typename T>
void StateStack::registerState(States::ID stateID) {
	mFactories[stateID] = [this] () {
		return State::Ptr(new T(*this, mContext));
	};
}