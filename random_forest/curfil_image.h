#ifndef IMAGE_H_
#define IMAGE_H_

#include <memory>


/**
 * A tuple of a RGBD image and an according labeling.
 * @ingroup image_related
 */
class LabeledRGBDImage {
 public:
  // RGBD image
  std::shared_ptr<RGBDImage> rgbd_image;
    /**
     * the label iamge component
     */
    boost::shared_ptr<LabelImage> labelImage;

public:

    LabeledRGBDImage() :
            rgbdImage(), labelImage() {
    }

    /**
     * creates a LabeledRGBDImage object out of an rgbd and label images
     */
    LabeledRGBDImage(const boost::shared_ptr<RGBDImage>& rgbdImage,
            const boost::shared_ptr<LabelImage>& labelImage);

    /**
     * @return the total memory usage of the RGBD image as well as the according label image
     */
    size_t getSizeInMemory() const {
        return rgbdImage->getSizeInMemory() + labelImage->getSizeInMemory();
    }

    /**
     * @return the RGBD image component
     */
    const RGBDImage& getRGBDImage() const {
        return *(rgbdImage.get());
    }

    /**
     * @return the label image component
     */
    const LabelImage& getLabelImage() const {
        return *(labelImage.get());
    }

    /**
     * @return the width of the RGBD image
     */
    int getWidth() const {
        return rgbdImage->getWidth();
    }

    /**
     * @return the height of the RGBD image
     */
    int getHeight() const {
        return rgbdImage->getHeight();
    }

    /**
     * resize the image to new depth and height and uses the provided label for padding
     */
    void resizeImage(int newWidth, int newHeight, LabelType paddingLabel) const;

    /**
     * calculate the image integral
     */
    void calculateIntegral() const;

};


/**
 * Convenience function to load and convert a RGBD image and the according label image
 */
LabeledRGBDImage loadImagePair(const std::string& filename, bool useCIELab, bool useDepthImages,  bool useDepthFilling,
        bool calculateIntegralImages = true);

/**
 * Convenience function to find all files in the given folder that match the required filename schema.
 * See the README for the filename schema.
 */
std::vector<std::string> listImageFilenames(const std::string& folder);

/**
 * Convenience function to find all files in the given folder that match the required filename schema.
 * See the README for the filename schema.
 */
std::vector<LabeledRGBDImage> loadImages(const std::string& folder, bool useCIELab, bool useDepthImages, bool useDepthFilling, const std::vector<std::string>& ignoredColors, size_t& numLabels);

}


#endif
