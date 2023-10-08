#include <stdio.h>
#include <stdlib.h>

struct ITEM
{
    int item_id;
    float item_profit;
    float item_weight;
    float profit_weight_ratio;
};

void swap(struct ITEM *a, struct ITEM *b)
{
    struct ITEM temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(struct ITEM arr[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].profit_weight_ratio > arr[largest].profit_weight_ratio)
        largest = left;

    if (right < n && arr[right].profit_weight_ratio > arr[largest].profit_weight_ratio)
        largest = right;

    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
        maxHeapify(arr, n, largest);
    }
}

void buildMaxHeap(struct ITEM arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        maxHeapify(arr, n, i);
}

void heapSort(struct ITEM arr[], int n)
{
    buildMaxHeap(arr, n);

    for (int i = n - 1; i >= 0; i--)
    {
        swap(&arr[0], &arr[i]);
        maxHeapify(arr, i, 0);
    }
}

int main()
{
    int n;
    printf("Enter the number of items: ");
    scanf("%d", &n);

    struct ITEM *items = (struct ITEM *)malloc(n * sizeof(struct ITEM));
    float knapsack_capacity;

    for (int i = 0; i < n; i++)
    {
        items[i].item_id = i + 1;
        printf("Enter the profit and weight of item no %d: ", i + 1);
        scanf("%f %f", &items[i].item_profit, &items[i].item_weight);
        items[i].profit_weight_ratio = items[i].item_profit / items[i].item_weight;
    }

    printf("Enter the capacity of knapsack: ");
    scanf("%f", &knapsack_capacity);

    heapSort(items, n);

    float total_profit = 0.0;
    printf("\nItem No\tprofit\tWeight\tAmount to be taken\n");

    for (int i = 0; i < n; i++)
    {
        float amount_taken = (knapsack_capacity >= items[i].item_weight) ? 1.0 : (knapsack_capacity / items[i].item_weight);
        printf("%d\t%.6f\t%.6f\t%.6f\n", items[i].item_id, items[i].item_profit, items[i].item_weight, amount_taken);
        total_profit += amount_taken * items[i].item_profit;
        knapsack_capacity -= amount_taken * items[i].item_weight;
    }

    printf("Maximum profit: %.6f\n", total_profit);

    free(items);

    return 0;
}
