#include <xmtc.h>

/* Parallel Euler Tour and Pointer Jumping */

int next_a[M], next_b[M];
int dist_a[M], dist_b[M];

int main()
{
    int last, round, steps;

    /* Step 1: Construct Euler tour */

    last = ptr[V[root] + deg[root] - 1];

    spawn(0, M - 1)
    {
        int e;
        int u;
        int rev;
        int first;
        int offset;
        int successor;

        e = $;

        u = E[e][1];
        rev = ptr[e];
        first = V[u];
        offset = rev - first;
        successor = first + ((offset + 1) % deg[u]);

        next_a[e] = (e == last) ? NIL : successor;
        dist_a[e] = (e == last) ? 0 : 1;
    }

    /* Step 2: Parallel pointer jumping */

    steps = 1;
    round = 0;

    while (steps < M)
    {
        if (round % 2 == 0)
        {
            spawn(0, M - 1)
            {
                int e;
                e = $;

                if (next_a[e] == NIL)
                {
                    next_b[e] = NIL;
                    dist_b[e] = dist_a[e];
                }
                else
                {
                    next_b[e] = next_a[next_a[e]];
                    dist_b[e] = dist_a[e] + dist_a[next_a[e]];
                }
            }
        }
        else
        {
            spawn(0, M - 1)
            {
                int e;
                e = $;

                if (next_b[e] == NIL)
                {
                    next_a[e] = NIL;
                    dist_a[e] = dist_b[e];
                }
                else
                {
                    next_a[e] = next_b[next_b[e]];
                    dist_a[e] = dist_b[e] + dist_b[next_b[e]];
                }
            }
        }

        steps = steps * 2;
        round++;
    }

    /* Step 3: Select edges directed toward root */

    if (round % 2 == 0)
    {
        spawn(0, M - 1)
        {
            int e;
            e = $;
            used[e] = (dist_a[e] < dist_a[ptr[e]]) ? 1 : 0;
        }
    }
    else
    {
        spawn(0, M - 1)
        {
            int e;
            e = $;
            used[e] = (dist_b[e] < dist_b[ptr[e]]) ? 1 : 0;
        }
    }


    return 0;
}