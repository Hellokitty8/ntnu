'''
Forward-Backward Algorithm
Exercise 2 - TDT4171
Spring 2014
'''

import numpy as np
     
def forward(observations, dynamics, prior):
    '''
    Calculate forward messages
    f_{0,0} = prior [0.5, 0.5]
    For all observations
    Calculate the forward messages using the forward_step function    
    '''
    
    pass_on = prior
    forward_messages = [prior]
    
    for observation in observations:
        
        pass_on = forward_step(pass_on, observation, dynamics)          
        forward_messages.append(pass_on)
        
    return np.row_stack(forward_messages)


def backward(observations, dynamics, posterior):
    '''
    Calculate backward messages
    b_{5,5} = posterior [1.0, 1.0]
    For all observations, in reverse order
    Calculate the backward messages using the backward_step function
    Reverse the message list (as they are stored in descending order)
    '''
    
    pass_on = posterior
    backward_messages = [posterior]
    
    for observation in reversed(observations):
        
        pass_on = backward_step(pass_on, observation, dynamics)
        backward_messages.append(pass_on)
    
    backward_messages.reverse()
    return np.row_stack(backward_messages)
    
def forward_backward(evidence, dynamics, prior, posterior):
    '''
    Forward backward algorithm
    1. Compute forward messages
    2. Compute backward messages
    3. Compute the (normalized) smoothed probability values
    '''
    forward_messages = forward(evidence, dynamics, prior)
    backward_messages = backward(evidence, dynamics, posterior)
    return normalize2(np.multiply(forward_messages, backward_messages))
    
def forward_step(old_message, observation, dynamics):
    '''
    Forward step: observation * dynamics * old_message (For step 2 use forward probability of step 1)
    '''
    return normalize(np.dot(np.dot(observation, dynamics), old_message))
     
def backward_step(old_message, observation, dynamics):
    '''
    Backward step: dynamics * observation * old_message (For step 4 use backward probability of step 5)
    '''
    return normalize(np.dot(np.dot(dynamics, observation), old_message))

def normalize(array):
    '''
    Normalize a 2x1 matrix
    '''
    return array/np.sum(array)

def normalize2(array):
    '''
    Normalize each row of a 2xn matrix
    '''
    row_sums = array.sum(axis=1)
    return array / row_sums[:, np.newaxis]

#Starting forward probabilities (f_{0,0})
prior = [.5, .5]
#Starting backward probabilities (b_{5,5})
posterior = [1.0, 1.0]
#Transition model
dynamics = np.array([[.7, .3], [.3, .7]])
#Observation model, given umbrella
umbrella = np.array([[.9, .0], [.0, .2]])
#Observation model, given no umbrella
no_umbrella = np.array([[.1, .0], [.0, .8]])
#Sequence of events
observations = [umbrella, umbrella, no_umbrella, umbrella, umbrella]   

if __name__ == '__main__':
    print ('Forward Messages:')
    print forward(observations, dynamics, prior)
    print ('Backward Messages:')
    print backward(observations, dynamics, posterior)
    print ('Smoothed Messages:')
    print forward_backward(observations, dynamics, prior, posterior)