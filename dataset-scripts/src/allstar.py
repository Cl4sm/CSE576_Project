##############################################################################################
# Copyright 2020 The Johns Hopkins University Applied Physics Laboratory LLC
# All rights reserved.
# Permission is hereby granted, free of charge, to any person obtaining a copy of this 
# software and associated documentation files (the "Software"), to deal in the Software 
# without restriction, including without limitation the rights to use, copy, modify, 
# merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
# permit persons to whom the Software is furnished to do so.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
# OR OTHER DEALINGS IN THE SOFTWARE.
#
# HAVE A NICE DAY.

import requests
from urllib.parse import urljoin

#class for interacting with ALLSTAR repository
#can use alternate URL if you have cloned ALLSTAR locally

#Example:
#import allstar
#
#repo = allstar.AllstarRepo("armel")
#for pkg in repo.packages():
#  for b in pkg.binaries():

class AllstarRepo(object):
    def __init__(self, arch, base_url='https://allstar.jhuapl.edu'):
        self.arch = arch
        self.base_url = base_url
        self.rsession = requests.Session()


        self.packages = {}
        self._generate_package_list()

    def _generate_package_list(self):
        for part in range(1,5):
            url = urljoin(self.base_url, '/repo/jessie-list-p{}-final.txt'.format(part))
            r = self.rsession.get(url)

            for pkg in r.text.split():
                self.packages[pkg] = part


    def _package_part(self, pkg):
        return self.packages[pkg]

    def _package_index(self, pkg):
        index_url = self._package_index_url(pkg)
        return self.rsession.get(index_url).json()

    def _package_index_raw(self, pkg):
        index_url = self._package_index_url(pkg)
        return self.rsession.get(index_url).text

    def _package_index_url(self, pkg):
        pkg_url = urljoin(self.base_url, '/repo/p{}/{}/{}/'.format(self._package_part(pkg),
                                                                   self.arch, pkg))
        
        return urljoin(pkg_url, 'index.json')

    def package_list(self):
        return list(self.packages.keys())

    def package_binaries_details(self, pkg, pkg_info):
        binaries = []
        for i in range(0, len(pkg_info['binaries'])):
            f = pkg_info['binaries'][i]['file']
            binary_url = urljoin(self.base_url,
                                 '/repo/p{}/{}/{}/{}'.format(self._package_part(pkg),
                                                             self.arch, pkg, f))
            binaries.append({'name': pkg_info['binaries'][i]['name'],
                             'url': binary_url})

        return binaries

    def package_binaries(self, pkg):
        binaries = []
        index = self._package_index(pkg)

        for i in range(0, len(index['binaries'])):
            f = index['binaries'][i]['file']
            binary_url = urljoin(self.base_url,
                                 '/repo/p{}/{}/{}/{}'.format(self._package_part(pkg),
                                                             self.arch, pkg, f))
            r = self.rsession.get(binary_url)
            binaries.append({'name': index['binaries'][i]['name'],
                             'content': r.content})

        return binaries

    def package_gimples(self, pkg):
        gimples = []
        index = self._package_index(pkg)

        for i in range(0, len(index['binaries'])):
            for j in range(0, len(index['binaries'][i]['units'])):
                if('gimple' in index['binaries'][i]['units'][j]):
                    g = index['binaries'][i]['units'][j]['gimple']
                    gimple_url = urljoin(self.base_url,
                        '/repo/p{}/{}/{}/{}'.format(self._package_part(pkg),
                                                    self.arch,
                                                    pkg,
                                                    g))
                    r = self.rsession.get(gimple_url)
                    gimples.append({'name': index['binaries'][i]['units'][j]['name'],
                                    'content': r.content})
            
        return gimples