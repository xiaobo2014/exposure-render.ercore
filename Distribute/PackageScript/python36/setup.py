import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="OffScreenFFR_cpy36",
    version="0.9",
    #author="Example Author",
    #author_email="author@example.com",
    # url="https://github.com/pypa/sampleproject",
    description="This is a package for vascular modeling",
    long_description=long_description,
    long_description_content_type="text/markdown",
    packages=setuptools.find_packages(),
    include_package_data=True,
    package_data={'OffScreenFFR': ['_OffScreenFFR.so']},
    # package_data={'lib':['*.so']},
    # package_dir = {'': ''},#not use
    # package_data={'example_pkg': ['lib/libvtkIOPLY-6.2.so.1']},
    # data_files=[('lib', ['lib/libvtkIOPLY-6.2.so.1'])],
    classifiers=[
        "Programming Language :: Python :: 3.6",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
    ],
    python_requires='>=3.6, <3.7',
)
