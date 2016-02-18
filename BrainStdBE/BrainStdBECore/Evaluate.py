import numpy as np
size = 1;

def evaluate(formula, PARAS, STATES):        
    return eval(formula)    

def evaluate_single(formula):        
    return eval(formula)
    
def exp(f):
    return np.exp(f)
    
def log(f):
    return np.log(f)
    
def roundup(f, d):
    return np.round(f,d)

def floor(f):
    return np.floor(f)

def ceil(f):
    return np.ceil(f)
    
def randfloat():
    return np.random.sample(size)
    
def randnorm():
    return np.random.randn(size) 
    
def randint(low, high):
    return np.random.randint(low, high, size)
    
def binomial(n, p):
    return np.random.binomial(n, p, size)

def cauchy():
    return np.randomstandard_cauchy(size)

def chisquare(df):
    return np.random.chisquare(df, size)
    
def dirichlet(alpha):
    return np.random.dirichlet(alpha, size)
    
def exponential(scale):
    return np.random.exponential(scale, size)

def gamma(shape, scale):
    return np.random.gamma(shape, scale, size)
    
def geometric(p):
    return np.random.geometric(p, size)
    
def gumbel(loc, scale):
    return np.random.gumbel(loc, scale, size)
    
def laplace(loc, scale):
    return np.random.laplace(loc, scale, size)
    
def logistic(loc, scale):
    return np.random.logistic(loc, scale, size)

def lognormal(mean, sigma):
    return np.random.lognormal(mean, sigma, size)

def normal(loc, scale):
    return np.random.normal(loc, scale, size)
    
def pareto(a):
    return np.random.pareto(a, size)
    
def poisson(lam):
    return np.random.poisson(lam, size)
    
def power(a):
    return np.random.power(a, size)
    
def rayleigh(scale):
    return np.random.rayleigh(scale, size)
    
def studentt(df):
    return np.random.standard_t(df, size)
    
def uniform(low, high):
    return np.random.uniform(low, high, size)
    
def vonmises(mu, kappa):
    return np.random.vonmises(mu, kappa, size)
    
def wald(mean, scale):
    return np.random.wald(mean, scale, size)
    
def weibull(a):
    return np.random.weibull(a, size)
    
def zipf(a):
    return np.random.zipf(a, size)
    
def greater(a, b, c, d):
    if a > b:
        return c
    else:
        return d
        
def greatereq(a, b, c, d):
    if a >= b:
        return c
    else:
        return d

def less(a, b, c, d):
    if a < b:
        return c
    else:
        return d
        
def lesseq(a, b, c, d):
    if a <= b:
        return c
    else:
        return d
              
def equal(a, b, c, d):
    if a == b:
        return c
    else:
        return d
        
def notequal(a, b, c, d):
    if a != b:
        return c
    else:
        return d