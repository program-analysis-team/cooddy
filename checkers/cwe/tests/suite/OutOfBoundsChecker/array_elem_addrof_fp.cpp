
void boo()
{
    int arr[10];
    int* it = &arr[0];
    while (it < &arr[10]) {  // no OOB here
        int x = *it;
        it++;
    }
}
