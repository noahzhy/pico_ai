import os, sys, glob

from PIL import Image


def img2gray(img_path):
    img = Image.open(img_path)
    img = img.convert('L')
    img.save(img_path)


# images to one header file
def img2array(img_paths, h_path):
    for img_path in img_paths:
        os.system('xxd -i %s >> %s' % (img_path, h_path))


def tflite2array(tflite_path, h_path):
    result = os.popen('cat %s | xxd -i' % tflite_path).read()
    with open(h_path, 'w+', encoding='utf-8') as f:
        f.write("""#ifndef _TFLITE_MODEL_H_\n""")
        f.write("""#define _TFLITE_MODEL_H_\n""")
        f.write("""alignas(8) const unsigned char tflite_model[] = {\n""")
        f.write(result)
        f.write("""};\n""")
        f.write("""#endif""")
    

if __name__ == '__main__':
    img_dir = 'lpr/data'
    h_path = 'lpr/images.h'
    img_paths = glob.glob(os.path.join(img_dir, '*.jpg'))
    # img2array(img_paths, h_path)
    tflite_path = 'lpr/data/model.tflite'
    save_path = 'lpr/src/tflite_model.h'
    tflite2array(tflite_path, save_path)
