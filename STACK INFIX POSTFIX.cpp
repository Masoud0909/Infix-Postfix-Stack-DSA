#include <iostream>

// Define a template struct for nodes in the stack
template <typename T>
struct StackNode {
	T val;														// Store the value of the node
	StackNode* next = nullptr;									// Pointer to the next node in the stack
	StackNode(T val) : val(val), next(nullptr) {}				// Constructor to initialize the node with a value
};

// Define a template class for implementing a stack
template <typename T>
class Stack {
public:
	StackNode<T>* head = nullptr;								// Pointer to the top node of the stack
	/*BIG FIVE FOR THE STACK SINCE IT USES LINKED LIST*/
//Default constructor
	Stack() : head{ nullptr } {}

	// Copy Constructor
	Stack(const Stack<T>& other) {
		StackNode<T>* temp = other.head;						 // Pointer to traverse the 'other' stack
		StackNode<T>* prev = nullptr;							 // Pointer to keep track of the previously created node in the current stacj
		head = nullptr;											 // Ensure the head of the new stack is initially nullptr
		while (temp != nullptr) {								 // Traverse the 'other' stack
			StackNode <T>* newNode = new StackNode<T>(temp->val);// Create a new node with the same value as the corresponding node in 'other'
			if (prev == nullptr)								 // If it's the first node being created in the new stack
			{
				head = newNode;									 // Set the newNode the head of the new stack
				prev = newNode;									 // Update the 'prev' pointer to the newly created node
			}
			else {
				prev->next = newNode;							 // Link the new node to the previous one next
				prev = newNode;									 //Update the 'prev' pointer to the newly created node
			}
			temp = temp->next;									 //moveo the next node in the 'other' stack
		}
		//head = clone(other.head);
	}
	/*StackNode<T>* clone(StackNode <T>* node) {				//RECURSION FUNCTION INSTEAD OF ABOVE CODE
	if (node != nullptr)
		return new StackNode<T>(node->val, clone(node->next));
	else return nullptr;
}*/

// Copy Assignment Operator
	Stack& operator=(const Stack& other) {
		if (this != &other) {										//to avoid deleting and copying from the same object
			remove();												//to ensure the current stack is not empty before deallocating memory
			StackNode<T>* temp = other.head;						// Pointer to traverse the 'other' stack
			StackNode<T>* prev = nullptr;							// Pointer to keep track of the previously created node in the current stack
			while (temp != nullptr) {								// copying values from other node to new one deeply
				StackNode<T>* newNode = new StackNode<T>(temp->val);
				if (head == nullptr) {
					head = newNode;									//set the newnode the head of the newstack
					prev = newNode;									//Update the prev pointer to the newly created node
				}
				else {
					prev->next = newNode;							//Link the new node to the previus one
					prev = newNode;									//update the prev node to the new one
				}
				temp = temp->next;									//move to the next node in the 'other' stack
			}
		}
		return *this;
	}

	// Move Constructor
	Stack(Stack&& other) {
		if (this != &other)											// Checking if the other stack has any valu
		{
			if (head != nullptr) {									//check if the current stack is not empty
				remove();											//delete the current stack
			}
			head = other.head;										//stealing other stack ownership
			other.head = nullptr;									//other stack head is now nullptr
		}
	}
	//Move Assignment Operator
	Stack& operator=(Stack&& other) {
		if (this != &other)											//avoiding self-assignment
		{
			if (head != nullptr)									//check if the current stack is not empty
			{
				remove();											//delete the current stack
			}
			head = other.head;										//Taking its value
			other.head = nullptr;									//other stack it has no value anymore
		}
		return*this;
	}

	void push(T elem) {											// Method to add an element on top of the stack
		// If the stack is empty, create a new node and	make it the top
		if (head == nullptr) {
			head = new StackNode<T>(elem);
		}
		else {													// If the stack is not empty, create a new node and link it to the current top
			StackNode<T>* newNode = new StackNode<T>(elem);
			newNode->next = this->head;							// Point the new node to the current top
			this->head = newNode;								// Make the new node the new top
		}
	}
	T pop() {													// Method to remove and return the top element of the stack
		// If the stack is empty, return -1 indicating an error condition
	// If the stack is not empty, remove the top element and return its value
		T temp = head->val;									// Store the value of the top node
		StackNode<T>* newHead = head->next;					// Point to the next node as the new top
		delete head;										// Delete the previous top node
		head = newHead;										// Update the top pointer
		return temp;										// Return the value of the removed node
	}


	// Method to get the value of the top element of the stack without removing it
	T top() {
		return head->val;										// Return the value of the top node
	}

	void remove() {
		while (head != nullptr) {
			StackNode<T>* temp = head->next;
			delete head;
			head = temp;
		}
	}
	bool isEmpty() {
		return head == nullptr;
	}

	~Stack() {													//Desructor of Stack
		remove();
	}
};

// Class for handling expressions
class Expression {
public:
	enum class Direction
	{
		infix, Postfix
	};

	Expression(std::string input, Direction direction) {		// Constructor to initialize an expression based on input and direction
		if (direction == Expression::Direction::infix)			// Store the input as either infix or postfix based on direction
			infix = input;
		else if (direction == Expression::Direction::Postfix)
			postfix = input;
	}

	//Method to convert infix expression to postfix
	const std::string ToPostfix()
	{
		Stack<char>* opStack = new Stack <char>();				//create a stack to hold operator
		postfix = "";								//initilaize an empty string for the postfix expression
		int n = infix.length();									//stroing the length of infix expression
		for (int i = 0; i < n; i++) {							//scannng the infix expression from left to right
			char currentChar = infix[i];						//Get the current character
			switch (currentChar) {
			case '+':
			case '-':
			case '*':
			case '/':
				//if the current character is an operator, pop operators from the stack and append them to postfix until the stack is emoty ot the top operator is '('
				while (!opStack->isEmpty() && opStack->top() != '(') {
					postfix += opStack->pop();
				}
				opStack->push(currentChar);						//push the current character onto the stack
				break;
			case'(':
				opStack->push(currentChar);						//if the current character is '(', push to stack
				break;
			case')':											//if its')'pop operators from stack and append them to postfix until the top operartor is (
				while (!opStack->isEmpty() && opStack->top() != '(') {
					postfix += opStack->pop();
				}
				if (!opStack->isEmpty()) {
					opStack->pop();								//pop the '(' from the stack
				}
				break;
			default:											//if the current character is not operator or a paranthesis (i.e. it's an operand), append it to postfix
				postfix += currentChar;
				break;
			}
		}														//after scanning entire infix, pop any remaining operators from the stack and append them to posfix
		while (!opStack->isEmpty()) {
			postfix += opStack->pop();
		}
		delete opStack;											//deleting operator stack since we domnt need it anymore
		return postfix;
	}
	// Method to convert postfix expression to infix
	const std::string ToInfix()
	{
		Stack<std::string> strStack;
		for (int i = 0; i < postfix.length(); i++) {
			char current = postfix[i];
			if (current == '+' || current == '-' || current == '*' || current == '/') {
				std::string operand2 = strStack.pop();			//pop second operand from stack
				std::string operand1 = strStack.pop();			//pop first operand from stack
				std::string infix = "(" + operand1 + current + operand2 + ")";
				strStack.push(infix);							//push the resulting string back onto stack
			}
			else {
				strStack.push(std::string(1, current));																	 //if the currenct cchar not operator (it's operand ) push it onto the stack
			}
		}
		return strStack.pop();									//after scanning all postfix , we retunr the infix
	}

	// Method to evaluate the expression
	double evaluate()
	{
		double result = 0;
		Stack <double>numstack;
		for (int i = 0; i < postfix.length(); i++) {
			char current = postfix[i];
			if (current == '+' || current == '-' || current == '*' || current == '/') {
				double op2 = numstack.pop();					//pop the scond operand from the stack
				double op1 = numstack.pop();					//pop the first operand from the stack
				switch (current) {
				case '+':
					result = op1 + op2;
					break;
				case '-':
					result = op1 - op2;
					break;
				case '*':
					result = op1 * op2;
					break;
				case'/':
					result = op1 / op2;
					break;
				}
				numstack.push(result);
			}
			else                                              //if the char is not operator so its a digit , oush it on the stack
			{
				numstack.push(current - '0');					//convert char to double and push it onto the stack
			}
		}
		return result;									//convert done!
	}
private:
	std::string infix;											// Infix expression
	std::string postfix;										// Postfix expression
};
int main() {
	// Example usage of the stack with different data types
	Stack<int> intStack;
	Stack<int> doubleStack;

	//push some elements to int stack
	intStack.push(9);
	intStack.push(13);

	//push elements to double stack
	doubleStack.push(6.7);
	doubleStack.push(3.3);

	//pop an element from the int stack and print
	std::cout << "Popped from intStack: " << intStack.pop() << std::endl;

	//pop an element from the double stack and print
	std::cout << "Popped from doubleStack: " << doubleStack.pop() << std::endl;

	/*MAIN EXPRESSION TEST BEGINS HERE: */

	std::string input;
	char direction;

	// Prompt the user for the direction of conversion
	std::cout << "Enter 'i' for infix to postfix conversion, or 'p' for postfix to infix conversion: ";
	std::cin >> direction;

	// Prompt the user for the expression
	std::cout << "Enter your expression: ";
	std::cin >> input;

	if (direction == 'i') {
		// If the user entered 'i', convert from infix to postfix
		Expression::Direction dir = Expression::Direction::infix;
		Expression expr(input, dir);
		std::string postfix = expr.ToPostfix();
		std::cout << "The postfix expression is: " << postfix << std::endl;
		std::cout << "The result of the expression is: " << expr.evaluate() << std::endl;
	}
	else if (direction == 'p') {
		// If the user entered 'p', convert from postfix to infix
		Expression::Direction dir = Expression::Direction::Postfix;
		Expression expr(input, dir);
		std::string infix = expr.ToInfix();
		std::cout << "The infix expression is: " << infix << std::endl;
		std::cout << "The result of the expression is: " << expr.evaluate() << std::endl;
	}
	else {
		// If the user entered anything else, print an error message
		std::cout << "Invalid direction. Please enter 'i' for infix to postfix conversion, or 'p' for postfix to infix conversion." << std::endl;
	}
	return 0;
}
