# Model results changed unexpectedly

I have copied the data (confusion matix and the subsequent statistics) for each variant in each model (every adjusted parameters). It was found that (at least) the results from the generalized models (those including variants into training) were changed despite no additional parameters were adjusted.

|          |                             | Before                           |           |-| After                   |           |
|----------|-----------------------------|----------------------------------|-----------|-|-------------------------|-----------|
| accuracy |                             | default                          | optimized |-| default                 | optimized |
|          | Validation set              | 88.12%                           | 85.36%    |-| 87.34%                  | 87.41%    |
|          | Testing set-regular moves   | 87.72%                           | 90.30%    |-| 90.50%                  | 94.65%    |
|          | Testing set-jump            | 71.49%                           | 77.82%    |-| 74.65%                  | 78.42%    |
|          | Testing set-K               | 65.74%                           | 60.59%    |-| 65.54%                  | 72.67%    |
|          | Testing set-left            | 87.92%                           | 86.53%    |-| 89.11%                  | 93.86%    |
|          | Testing set-MMA             | 63.17%                           | 69.11%    |-| 64.55%                  | 66.53%    |
|          | Testing set-R               | 60.79%                           | 57.43%    |-| 75.25%                  | 70.69%    |
|          | Testing set-M               | 74.65%                           | 73.86%    |-| 74.85%                  | 87.92%    |
|          | Testing set-All but regular | nil                              | nil       |-| nil                     | nil       |
|          | Testing set-All             | 73.07%                           | 73.66%    |-| 76.35%                  | 80.68%    |

