# discreteModel

## Installation
Use `install_github()` to install this package
```
remotes::install_github("thinhong/discreteModel")
```

## Example
### Simple SIR model without contact
Here we have to define:
* `transitions`: structure of the model or transitions among compartments, for instance S -> I -> R would be defined by `"S -> I"`, `"I -> R"` (note the `""` symbol, this will be read as strings), white space can be ignored here (i.e we can write `"S->I"`)
* `initialValues`: a vector defines initial values of compartments
* `distributions`: distributions of compartments, currently `exponential(rate)`, `gamma(scale, shape)`, and `weibull(scale, shape)` are available
```
library(discreteModel)

transitions <- c(
  "S -> E",
  "E -> I",
  "I -> R"
)

initialValues <- c(
  S = 999,
  E = 0,
  I = 1,
  R = 0
)

distributions <- c(E = weibull(scale = 2, shape = 5), 
                   I = exponential(rate = 1.5))

fmod <- runSim(daysFollowUp = 5000, errorTolerance = 0.01, timeStep = 0.001, 
               transmissionRate = 1.5, infectiousComps = "I", 
               transitions = transitions,
               initialValues = initialValues, distributions = distributions)
```

### Contact between locations
We now add another parameter `contacts`: a list of contact matrices. If we have contact between locations, `initialValues` and `distributions` will be lists of locations. The name of elements of `initialValues` and `distributions` need to match the dimension names of the matrices.

This script will simulate a SIR model of 2 locations: Ho Chi Minh and Ha Noi.
```
M_location <- matrix(c(0.85, 0.1, 0.1, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("HCM", "HN"), c("HCM", "HN")))

transitions <- c(
  "S -> I", 
  "I -> R"
)

initialValues <- list(
  HCM = c(
    S = 999,
    I = 1,
    R = 0),
  HN = c(
    S = 699,
    I = 0,
    R = 0)
)

distributions <- list(
  HCM = c(I = gamma(scale = 2, shape = 5)),
  HN = c(I = weibull(scale = 2, shape = 5))
)

fmod <- runSim(daysFollowUp = 5000, errorTolerance = 0.01, timeStep = 0.001, 
               transmissionRate = 1.5, infectiousComps = "I", 
               contacts = M_location, transitions = transitions,
               initialValues = initialValues, distributions = distributions)
```

If we only have one type of contact, we can directly parse the contact matrix into parameter `contacts` of `runSim()`
```
M_location <- matrix(c(0.85, 0.1, 0.1, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("HCM", "HN"), c("HCM", "HN")))

fmod <- runSim(daysFollowUp = 5000, errorTolerance = 0.01, timeStep = 0.001, 
               transmissionRate = 1.5, infectiousComps = "I", 
               contacts = M_location, transitions = transitions,
               initialValues = initialValues, distributions = distributions)
```

If we have more than one type of contact:
* `contacts` must be defined as a list of contact matrices
```
M_location <- matrix(c(0.85, 0.1, 0.1, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("HCM", "HN"), c("HCM", "HN")))
M_gender <- matrix(c(0.5, 0.75, 0.75, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("M", "F"), c("M", "F")))

contacts <- list(M_location, M_gender)
```
* Use `.` to define the names of elements inside `initialValues` and `distributions`

Full input should be defined as follows
```
M_location <- matrix(c(0.85, 0.1, 0.1, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("HCM", "HN"), c("HCM", "HN")))
M_gender <- matrix(c(0.5, 0.75, 0.75, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("M", "F"), c("M", "F")))

contacts <- list(M_location, M_gender)

transitions <- c(
  "S -> I", 
  "I -> R"
)

initialValues <- list(
  HCM.M = c(
    S = 999,
    I = 1,
    R = 0),
  HCM.F = c(
    S = 699,
    I = 0,
    R = 0),
  HN.M = c(
    S = 999,
    I = 1,
    R = 0),
  HN.F = c(
    S = 699,
    I = 0,
    R = 0)
)

distributions <- list(
  HCM.M = c(I = gamma(scale = 2, shape = 5)),
  HCM.F = c(I = gamma(scale = 2, shape = 5)),
  HN.M = c(I = weibull(scale = 2, shape = 5)),
  HN.F = c(I = weibull(scale = 2, shape = 5))
)

fmod <- runSim(daysFollowUp = 20000, errorTolerance = 0.01, timeStep = 0.001, 
               transmissionRate = 1.5, infectiousComps = "I", 
               contacts = contacts, transitions = transitions,
               initialValues = initialValues, distributions = distributions)
```

### Visualization
We can plot the data frame generated by `runSim()` function with `plot()`. The `plot()` function use ggplot2 to visualize. If we have groups that contact with others, it automatically plots multi-panel using `facet_wrap()`.
```
plot(fmod)
```

### Helper function
When there are many compartments, or many contacts, use this `inputHelper()` to assist the definition of initialValues and distributions
```
M_location <- matrix(c(0.85, 0.1, 0.1, 0.95), nrow = 2, ncol = 2, 
                     dimnames = list(c("HCM", "HN"), c("HCM", "HN")))
M_age <- matrix(c(0.5, 0.6, 0.7, 0.6, 0.4, 0.3, 0.7, 0.3, 0.8), nrow = 3, ncol = 3, 
                     dimnames = list(c("young", "middle", "older"), c("young", "middle", "older")))

contacts <- list(M_location, M_age)

transitions <- c(
  "S -> E", 
  "E -> A",
  "A -> I",
  "I -> D",
  "I -> R"
)

inputHelper(contacts = contacts, transitions = transitions)
```

Results
```
# Place these codes under the transitions and contacts you have written
# Set up the initial values for all compartments
initialValues <- list(
  HCM.middle = c(S = , E = , A = , I = , D = , R = ),
  HCM.older = c(S = , E = , A = , I = , D = , R = ),
  HCM.young = c(S = , E = , A = , I = , D = , R = ),
  HN.middle = c(S = , E = , A = , I = , D = , R = ),
  HN.older = c(S = , E = , A = , I = , D = , R = ),
  HN.young = c(S = , E = , A = , I = , D = , R = )
)

# Set up the distributions for all compartments, any compartment without distribution can be deleted
distributions <- list(
  HCM.middle = c(S = , E = , A = , I = , D = , R = ),
  HCM.older = c(S = , E = , A = , I = , D = , R = ),
  HCM.young = c(S = , E = , A = , I = , D = , R = ),
  HN.middle = c(S = , E = , A = , I = , D = , R = ),
  HN.older = c(S = , E = , A = , I = , D = , R = ),
  HN.young = c(S = , E = , A = , I = , D = , R = )
)

# Insert all parameters here, please use ?runSim for more details
fmod <- runSim(daysFollowUp = , errorTolerance = , timeStep = , 
               transmissionRate = , infectiousComps = , 
               contacts = , transitions = ,
               initialValues = initialValues, distributions = distributions)
```