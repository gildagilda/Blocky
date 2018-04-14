#include "Block.h"

// Constructor only dependant on preHash
Block::Block(std::string prevHash, int id) {
	// parameters assigning
	this->id = id;
	this->prevHash = prevHash;
	this->nonce = 0;
	this->currHash = "";
	this->numTrans=0;
	this->transactions = new Transaction[numTrans];
	this->mined = false;
}

// adding a transaction to the Block
void Block::addTransaction(Transaction transToAdd) {
	Transaction *temp = new Transaction[this->numTrans+1]; // create larger array
	for (int i = 0; i < this->numTrans; i++) {
		//copy the old array of transactions
		*(temp + i) = *(this->transactions + i);
	}
	*(temp + this->numTrans) = transToAdd; // assign new Transaction
	this->transactions = temp; // change pointer
	this->numTrans++; // increment the number of transactions
}

// returns string for hashing and file writing
std::string Block::stringify(){
	std::string rets =
		std::to_string(this->id) + "|" +
		this->prevHash + "|" +
		std::to_string(this->nonce) + "|" +
		std::to_string(this->numTrans) + "|";
	// loop over all transactions
	for (int i = 0; i < this->numTrans; i++) {
		rets += std::to_string(i) + ":" + (this->transactions + i)->stringify() + "|";
	}
	return rets;
}

// hash the current block and update its currHash
std::string Block::hashBlock() {
	this->currHash = Util::Hash256(this->stringify());
	return this->currHash;
}

// returns string with all parameters for debugging and viewing
std::string Block::toString() {
	std::string rets =
		"id: " + std::to_string(this->id) + "\r\n" +
		"prevHash: " + this->prevHash + "\r\n" +
		"nonce: " + std::to_string(this->nonce) + "\r\n" +
		"mined: " + (this->mined ? "true" : "false") + "\r\n" +
		"currHash: " + this->currHash + "\r\n" +
		"numTrans: " + std::to_string(this->numTrans) + "\r\n";
	// loop over all transactions
	for (int i = 0; i < this->numTrans; i++) {
		rets += std::to_string(i)+": "+(this->transactions + i)->toString() +"\r\n";
	}
	return rets;
}

bool Block::mine(int difficulty) {
	puts("starting to mine...");
	printf("%s", ("raw data:\r\n" + this->stringify() + "\r\n").c_str());
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while (Util::numZeroHash(Util::Hash256(this->stringify()))<difficulty && this->nonce < INT64_MAX) {
		this->nonce++;
		this->hashBlock();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	this->mined = true;
	long long int eTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	printf("%s", ("elapsed time: "+std::to_string((double)eTime/1000000)+" seconds\r\n").c_str());
	return this->mined;
}
