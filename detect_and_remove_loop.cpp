/*
    Title: Detect and Remove Loop in a Linked List
    Author: Praneeth Reddy Mallupalli
    Description:
        This program detects and removes a cycle (loop) in a singly linked list
        using Floyd’s Cycle Detection Algorithm, also known as the Tortoise and Hare method.

    Mathematical intuition:
        Suppose the linked list has:
            - L  = distance from head to start of the loop
            - C  = length (circumference) of the loop
            - x  = distance from start of loop to meeting point inside the loop

        When slow and fast pointers meet:
            - Slow has traveled L + x steps.
            - Fast has traveled 2(L + x) steps.
        
        Since fast moves twice as fast:
            2(L + x) - (L + x) = nC   →   L + x = nC   →   L = nC - x
        
        This means if we start one pointer at head and another at the meeting point,
        and move both one step at a time, they’ll meet at the start of the loop.
*/

#include <iostream>
using namespace std;

// --------------------------
// Node definition
// --------------------------
struct Node {
    int data;
    Node* next;
    Node(int val): data(val), next(nullptr) {}
};

// -----------------------------------------------------
// STEP 1: Detect if a loop exists using Floyd’s Algorithm
// -----------------------------------------------------
Node* detectLoop(Node* head) {
    Node* slow = head;
    Node* fast = head;

    while (fast && fast->next) {
        slow = slow->next;         // moves 1 step
        fast = fast->next->next;   // moves 2 steps

        if (slow == fast) {
            /*
                Mathematical proof of why they meet:

                Let:
                    L = distance from head to start of loop
                    x = distance from start of loop to meeting point
                    C = loop length

                Then:
                    slow = L + x
                    fast = L + x + k*C   (since fast completes k full cycles)

                Since fast moves twice as fast:
                    2(L + x) = L + x + k*C  →  L + x = k*C  →  L = k*C - x

                Therefore, moving one pointer to head and one from meeting point
                and advancing both step by step will make them meet at loop start.
            */
            return slow; // meeting point inside the loop
        }
    }

    return nullptr; // No loop found
}

// -----------------------------------------------------
// STEP 2: Find the starting node of the loop
// -----------------------------------------------------
Node* findStartOfLoop(Node* head, Node* meetingPoint) {
    Node* ptr1 = head;
    Node* ptr2 = meetingPoint;

    // Move both one step at a time — they meet at the start of the loop
    while (ptr1 != ptr2) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    return ptr1; // start node of the loop
}

// -----------------------------------------------------
// STEP 3: Remove the loop
// -----------------------------------------------------
void removeLoop(Node* loopStart) {
    Node* temp = loopStart;

    // Traverse the loop until you reach the node before loopStart
    while (temp->next != loopStart)
        temp = temp->next;

    // Break the loop
    temp->next = nullptr;
}

// -----------------------------------------------------
// STEP 4: Combined function — detect and remove loop
// -----------------------------------------------------
void detectAndRemoveLoop(Node* head) {
    Node* meetingPoint = detectLoop(head);

    if (!meetingPoint) {
        cout << "No loop detected." << endl;
        return;
    }

    Node* loopStart = findStartOfLoop(head, meetingPoint);
    removeLoop(loopStart);

    cout << "Loop detected and removed at node with value: "
         << loopStart->data << endl;
}

// -----------------------------------------------------
// Helper functions for testing
// -----------------------------------------------------
void printList(Node* head) {
    while (head) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

// -----------------------------------------------------
// Example usage
// -----------------------------------------------------
int main() {
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);
    head->next->next->next = new Node(4);
    head->next->next->next->next = new Node(5);

    // Create a loop for testing: 5 → 3
    head->next->next->next->next->next = head->next->next;

    detectAndRemoveLoop(head);

    cout << "Linked list after loop removal: ";
    printList(head);

    return 0;
}

/*
    Output:
    Loop detected and removed at node with value: 3
    Linked list after loop removal: 1 2 3 4 5
*/
