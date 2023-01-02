import matplotlib.pyplot as plt
from skimage import exposure
from skimage.exposure import match_histograms
import cv2
import glob

inputdir = "./input/"
startnum = 3712


tarconcattex = []
matchedtex = []

reftex1 = cv2.imread(inputdir + str(startnum)+ "/textured_model_material0000_map_Kd.png")
reftex2 = cv2.imread(inputdir + str(startnum)+ "/textured_model_material0001_map_Kd.png")
refconcattex = cv2.vconcat(reftex1, reftex2)

for i in range(1,4):
    tex1 = cv2.imread(inputdir + str(startnum+i)+ "/textured_model_material0000_map_Kd.png")
    tex2 = cv2.imread(inputdir + str(startnum+i)+ "/textured_model_material0001_map_Kd.png")
    tarconcattex.append(cv2.vconcat(tex1, tex2))

for i in range(1,4):    
    matched_temp = match_histograms(tarconcattex[i], refconcattex ,
                               multichannel=True)            
    matchedtex.append(matched_temp)
    # fig, (ax1, ax2, ax3) = plt.subplots(nrows=1, ncols=3, 
    #                                     figsize=(8, 3),
    #                                     sharex=True, sharey=True)
    # for aa in (ax1, ax2, ax3):
    #     aa.set_axis_off()
    
    # ax1.imshow(image)
    # ax1.set_title('Source')
    # ax2.imshow(reference)
    # ax2.set_title('Reference')
    # ax3.imshow(matched_temp)
    # ax3.set_title('Matched')
    # plt.tight_layout()
    # plt.show()
    
    # fig, axes = plt.subplots(nrows=3, ncols=3, figsize=(8, 8))
    
    # for i, img in enumerate((image, reference, matched_temp)):
    #     for c, c_color in enumerate(('red', 'green', 'blue')):
    #         img_hist, bins = exposure.histogram(img[..., c], 
    #                                             source_range='dtype')
    #         axes[c, i].plot(bins, img_hist / img_hist.max())
    #         img_cdf, bins = exposure.cumulative_distribution(img[..., c])
    #         axes[c, i].plot(bins, img_cdf)
    #         axes[c, 0].set_ylabel(c_color)
    
    # axes[0, 0].set_title('Source')
    # axes[0, 1].set_title('Reference')
    # axes[0, 2].set_title('Matched')
    
    # plt.tight_layout()
    # plt.show()


for i in range(len(matchedtex)):
    cv2.imwrite("221217matched/" + str(i) + ".png", matchedtex[i])


  
