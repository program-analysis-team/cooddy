bool is_url_nrf(const char* url)
{
    if (url == nullptr) {
        return false;
    }
    /* POTENTIAL FLAW: Access buffer may be out of bounds */
    return url[2] == 'n';
}

void ArrayIndex_WithoutCheckExternalData_GoodCase02()
{
    const char* url = "1";
    bool is_url = is_url_nrf(url);
}
