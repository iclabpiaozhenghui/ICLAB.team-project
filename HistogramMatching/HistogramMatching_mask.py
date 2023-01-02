import matplotlib.pyplot as plt
import numpy as np
from skimage import exposure
from skimage.exposure import match_histograms
import skimage.metrics
import cv2
  

def cosinesim(a,b):
    sim = np.dot(a,b)/(np.linalg.norm(a) * np.linalg.norm(b))
    return sim

def histeq(img):
    hist, bins = np.histogram(img, 256,[0,256])
    cdf = hist.cumsum()
    cdf_normalized = cdf * float(hist.max()) / cdf.max()

# reading main image
img1 = cv2.imread("./head_blue/1_img0.png")
# mask_img1 = cv2.inRange(img1, (255,0,0), (255,0,0))
# mask_img1 = 255 - mask_img1
mask_img1 = cv2.inRange(img1[..., 0], 255, 255)
print('No of Channel is: ' + str(img1.ndim))
  
# reading reference image
img2 = cv2.imread("./head_blue/0_img0.png")
# mask_img2 = cv2.inRange(img2, (255,0,0), (255,0,0))
# mask_img2 = 255 - mask_img2
mask_img2 = cv2.inRange(img2[..., 0], 255, 255)
print('No of Channel is: ' + str(img2.ndim))

# filteredimg1 = np.zeros(img1.shape, dtype=np.uint8)
# filteredimg2 = np.zeros(img1.shape, dtype=np.uint8)
mask_img1 = np.where(mask_img1 == 0)
mask_img2 = np.where(mask_img2 == 0)

# cv2.copyTo(img1, mask_img1, filteredimg1)
# cv2.copyTo(img2, mask_img2, filteredimg2)

filteredimg1 = img1[mask_img1]
filteredimg2 = img2[mask_img2]

matched = match_histograms(filteredimg1, filteredimg2 ,
                           multichannel=True)

fig, (ax1, ax2, ax3) = plt.subplots(nrows=1, ncols=3, 
                                    figsize=(8, 3),
                                    sharex=True, sharey=True)
  
for aa in (ax1, ax2, ax3):
    aa.set_axis_off()

# matched = np.resize([img1.shape,3], matched.all())
# cv2.imwrite("matched.bmp", matched)

ax1.imshow(img1)
ax1.set_title('Source')
ax2.imshow(img2)
ax2.set_title('Reference')
ax3.imshow(matched)
ax3.set_title('Matched')
# cv2.imwrite("Matched1_img0.png", matched)

plt.tight_layout()
plt.show()
  
fig, axes = plt.subplots(nrows=3, ncols=3, figsize=(8, 8))
  
dumphist = []
for i, img in enumerate((filteredimg1, filteredimg2, matched)):
    channelhist = []
    for c, c_color in enumerate(('red', 'green', 'blue')):
        img_hist, bins = exposure.histogram(img[..., c], 
                                            source_range='dtype')
        axes[c, i].plot(bins, img_hist / img_hist.max())
        img_cdf, bins = exposure.cumulative_distribution(img[..., c])
        axes[c, i].plot(bins, img_cdf)
        axes[c, 0].set_ylabel(c_color)
        channelhist.append(img_hist)
    dumphist.append(channelhist)

# for i, img in enumerate((filteredimg1, filteredimg2, matched)):    
#     for c, c_color in enumerate(('red', 'green', 'blue')):
#         img_hist, bins = exposure.histogram(img[..., c], 
#                                     source_range='dtype')

#         cdf = img_hist.cumsum()
#         cdf_normalized = cdf * float(img_hist.max()) / cdf.max()
        
#         axes[c, i].plot(bins, cdf_normalized / cdf_normalized.max())
#         img_cdf, bins = exposure.cumulative_distribution(img[..., c])
#         axes[c, i].plot(bins, img_cdf)
#         axes[c, 0].set_ylabel(c_color)

print("ref-src")
# print(np.sum(dumphist[1][0]) - np.sum(dumphist[0][0]))
# print(np.sum(dumphist[1][1]) - np.sum(dumphist[0][1]))
# print(np.sum(dumphist[1][2]) - np.sum(dumphist[0][2]))
# print(cosinesim(dumphist[1][0],dumphist[0][0]))
# print(cosinesim(dumphist[1][1],dumphist[0][1]))
# print(cosinesim(dumphist[1][2],dumphist[0][2]))

print("ref-matched")
# print(np.sum(dumphist[1][0]) - np.sum(dumphist[2][0]))
# print(np.sum(dumphist[1][1]) - np.sum(dumphist[2][1]))
# print(np.sum(dumphist[1][2]) - np.sum(dumphist[2][2]))
# print(cosinesim(dumphist[1][0],dumphist[2][0]))
# print(cosinesim(dumphist[1][1],dumphist[2][1]))
# print(cosinesim(dumphist[1][2],dumphist[2][2]))

axes[0, 0].set_title('Source')
axes[0, 1].set_title('Reference')
axes[0, 2].set_title('Matched')

plt.tight_layout()
plt.show()