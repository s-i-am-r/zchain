project('zchain', 'cpp')

openssl_dep = dependency('openssl', required: true)

src_files = ['chain.cpp']

inc_dirs = include_directories('.')

executable('chain', src_files, 
           include_directories: inc_dirs,
           dependencies: openssl_dep)
