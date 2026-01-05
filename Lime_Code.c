#include <stdio.h>   // For input and output
#include <stdlib.h>  // General utilities
#include <math.h>    // For fabs()

#define N_FEATURES 5 // Total number of features

/* ---------------------------------------------
   normalize()
   Converts a value into range 0 to 1
----------------------------------------------*/
float normalize(float value, float min, float max) {
    return (value - min) / (max - min);
}

/* ---------------------------------------------
   predict()
   Simple dummy model that calculates
   weighted sum of input features
----------------------------------------------*/
float predict(float input[]) {

    // Weights assigned to each feature
    float weights[N_FEATURES] = {0.2, 0.35, 0.25, 0.1, 0.1};
    float result = 0.0;

    // Calculate weighted sum
    for (int i = 0; i < N_FEATURES; i++) {
        result += input[i] * weights[i];
    }

    return result;
}

/* ---------------------------------------------
   simulate_explanation()
   Explains prediction using simplified LIME
----------------------------------------------*/
void simulate_explanation(float input[],
                          float age,
                          float salary,
                          float credit_score,
                          float loan_amount,
                          float dependents) {

    // Original prediction
    float baseline = predict(input);

    // Feature names for display
    char *feature_names[N_FEATURES] = {
        "Age", "Salary", "Credit Score", "Loan Amount", "Dependents"
    };

    float perturbed[N_FEATURES];   // Modified inputs
    float influences[N_FEATURES];  // Effect of each feature
    int indices[N_FEATURES];       // Used for sorting

    printf("\nOriginal Prediction: %.4f\n\n", baseline);

    /* Step 1: Change one feature at a time
       Step 2: Measure prediction change */
    for (int i = 0; i < N_FEATURES; i++) {

        // Copy original input
        for (int j = 0; j < N_FEATURES; j++) {
            perturbed[j] = input[j];
        }

        // Set one feature to maximum value
        perturbed[i] = 1.0;

        // Calculate influence
        influences[i] = fabs(predict(perturbed) - baseline);
        indices[i] = i;
    }

    /* Sort features by influence (descending order) */
    for (int i = 0; i < N_FEATURES - 1; i++) {
        for (int j = 0; j < N_FEATURES - i - 1; j++) {
            if (influences[indices[j]] < influences[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    // Display feature importance ranking
    printf("Feature Influence Ranking:\n");
    for (int i = 0; i < N_FEATURES; i++) {
        int idx = indices[i];
        printf("%d. %s : Change = %.4f\n",
               i + 1, feature_names[idx], influences[idx]);
    }

    /* Decision explanation */
    printf("\n");
    if (baseline >= 0.5) {
        printf("Loan is likely to be APPROVED.\n");
        printf("Reason: Good income and credit score.\n");
    } else {
        printf("Loan is NOT likely to be approved.\nReason: ");

        if (salary < 30000)
            printf("Low salary. ");
        if (loan_amount > 20000)
            printf("High loan amount. ");
        if (credit_score < 650)
            printf("Low credit score. ");
        if (dependents > 2)
            printf("More dependents. ");
        if (age < 21)
            printf("Young applicant. ");

        printf("\n");
    }
}

/* ---------------------------------------------
   main()
   Takes user input and runs the explanation
----------------------------------------------*/
int main() {

    float input[N_FEATURES];
    float age, salary, credit_score, loan_amount, dependents;

    // User input
    printf("Enter loan applicant details:\n");
    printf("1. Age (18-60): ");
    scanf("%f", &age);

    printf("2. Salary (10000 - 100000): ");
    scanf("%f", &salary);

    printf("3. Credit Score (300 - 900): ");
    scanf("%f", &credit_score);

    printf("4. Loan Amount (5000 - 50000): ");
    scanf("%f", &loan_amount);

    printf("5. Number of Dependents (0 - 5): ");
    scanf("%f", &dependents);

    // Normalize inputs
    input[0] = normalize(age, 18, 60);
    input[1] = normalize(salary, 10000, 100000);
    input[2] = normalize(credit_score, 300, 900);
    input[3] = normalize(loan_amount, 5000, 50000);
    input[4] = normalize(dependents, 0, 5);

    // Run simplified LIME explanation
    simulate_explanation(input,
                         age,
                         salary,
                         credit_score,
                         loan_amount,
                         dependents);

    return 0;
}
