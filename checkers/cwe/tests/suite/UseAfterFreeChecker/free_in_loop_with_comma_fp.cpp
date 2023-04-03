#include <stdlib.h>

typedef struct _A {
    struct _A *pstNext;
    struct _A *pstPrev;
} CLIST_HEAD_S;

void my_free(CLIST_HEAD_S *a)
{
    free(a);
}

#define CLIST_SCAN_SAFE(pstNode, pstTmp, pstPathStaticsList)                                                          \
    for ((pstNode) = (pstPathStaticsList)->pstNext, (pstTmp) = (pstNode)->pstNext; (pstNode) != (pstPathStaticsList); \
         (pstNode) = (pstTmp), (pstTmp) = (pstNode)->pstNext)

#define CLIST_RemoveSafe(pstNode)                                                               \
    do {                                                                                        \
        if (((((pstNode)) != ((pstNode))->pstNext) && (((pstNode)) != ((pstNode))->pstPrev))) { \
            do {                                                                                \
                ((pstNode))->pstPrev = ((pstNode));                                             \
                ((pstNode))->pstNext = ((pstNode));                                             \
            } while (0);                                                                        \
        }                                                                                       \
    } while (0)

void SOCK_APP_ProcSlavePathStatics(CLIST_HEAD_S *pstPathStaticsList)
{
    CLIST_HEAD_S *pstNode = NULL;
    CLIST_HEAD_S *pstTmp = NULL;
    CLIST_SCAN_SAFE(pstNode, pstTmp, pstPathStaticsList)
    {
        CLIST_RemoveSafe(pstNode);
        (void)my_free(pstNode);
    }
}
