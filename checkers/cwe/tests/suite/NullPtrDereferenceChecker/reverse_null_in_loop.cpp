struct A {
    A* next;
};
int foo(A* head)
{
    A* node = head->next;
    A* temp = node->next;
    while(node != nullptr) { //FP
    }
}
int foo2(A* head)
{
    A* node = head->next;
    A* temp = node;
    for(;node != nullptr;) { //FP
        node = node->next;
    }
    temp->next;
}
int foo3(A* head)
{
    A* node = nullptr;
    A* temp = nullptr;
    for (node = head->next, temp = node->next; node != nullptr; node = temp, temp = node->next) { //TP
    }
}
