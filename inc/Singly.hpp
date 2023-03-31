#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Entity.hpp>
#include <SinglyNode.hpp>

#include <memory>

class SinglyLinkedList : public Entity {
	public:
		explicit SinglyLinkedList(const FontHolder& fonts);
		SinglyNode* getHead();
		void randomGen();
		void pushBack(SinglyNode* newNode);
		int getSize();

	private:
		const FontHolder& mFonts;
		std::size_t mSize;
		SinglyNode* mHead;
};

#endif // SINGLYLINKEDLIST_HPP