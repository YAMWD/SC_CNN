import cv2
import idx2numpy
import numpy as np

base_dir = "/Users/yaozhiyan/desktop/projects/SC_CNN/data/MNIST"

train_images_dir = base_dir + "/train_images/"

train_labels_dir = base_dir + "/train_labels/"

test_images_dir = base_dir + "/test_images/"

test_labels_dir = base_dir + "/test_labels/"

if __name__ == '__main__':

    train_images = idx2numpy.convert_from_file(train_images_dir + 'train-images.idx3-ubyte')

    train_labels = idx2numpy.convert_from_file(train_labels_dir + 'train-labels.idx1-ubyte')

    test_images = idx2numpy.convert_from_file(test_images_dir + 't10k-images.idx3-ubyte')

    test_labels = idx2numpy.convert_from_file(test_labels_dir + 't10k-labels.idx1-ubyte')

    train_images_out = train_images.reshape(train_images.shape[0], -1)

    train_labels_out = train_labels.reshape(train_labels.shape[0], -1)
    
    test_images_out = test_images.reshape(test_images.shape[0], -1)
    
    test_labels_out = test_labels.reshape(test_labels.shape[0], -1)


    #saving as CSV
    np.savetxt(train_images_dir + 'train_images.out', train_images_out, delimiter = ' ', fmt = '%d')

    np.savetxt(train_labels_dir + 'train_labels.out', train_labels_out, delimiter = ' ', fmt = '%d')
    
    np.savetxt(test_images_dir + 'test_images.out', test_images_out, delimiter = ' ', fmt = '%d')
    
    np.savetxt(test_labels_dir + 'test_labels.out', test_labels_out, delimiter = ' ', fmt = '%d')


    '''
    #just for fun
    #manually check
    for i in range(10000):

        cv2.imshow("Image", train_image[i])

        print(train_label[i])

        cv2.waitKey(0)
    '''

