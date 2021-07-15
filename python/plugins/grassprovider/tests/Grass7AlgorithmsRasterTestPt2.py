# -*- coding: utf-8 -*-

"""
***************************************************************************
    Grass7AlgorithmsRasterTestPt2.py
    -----------------------------
    Date                 : May 2016
    Copyright            : (C) 2016 by Médéric Ribreux
    Email                : mederic dot ribreux at medspx dot fr
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Médéric Ribreux'
__date__ = 'May 2016'
__copyright__ = '(C) 2016, Médéric Ribreux'

import AlgorithmsTestBase

import nose2
import shutil
import os
import tempfile

from qgis.core import (
    QgsApplication,
    QgsProcessingContext
)
from qgis.testing import (
    start_app,
    unittest
)
from grassprovider.Grass7AlgorithmProvider import Grass7AlgorithmProvider
from grassprovider.Grass7Utils import Grass7Utils


testDataPath = os.path.join(os.path.dirname(__file__), 'testdata')


class TestGrass7AlgorithmsRasterTest(unittest.TestCase, AlgorithmsTestBase.AlgorithmsTest):

    @classmethod
    def setUpClass(cls):
        start_app()
        from processing.core.Processing import Processing
        Processing.initialize()
        cls.provider = Grass7AlgorithmProvider()
        QgsApplication.processingRegistry().addProvider(cls.provider)
        cls.cleanup_paths = []

        cls.temp_dir = tempfile.mkdtemp()
        cls.cleanup_paths.append(cls.temp_dir)

        assert Grass7Utils.installedVersion()

    @classmethod
    def tearDownClass(cls):
        from processing.core.Processing import Processing
        QgsApplication.processingRegistry().removeProvider(cls.provider)
        Processing.deinitialize()
        for path in cls.cleanup_paths:
            shutil.rmtree(path)

    def test_definition_file(self):
        return 'grass7_algorithms_raster_tests2.yaml'

    def testNeighbors(self):
        context = QgsProcessingContext()
        input_raster = os.path.join(testDataPath, 'float_raster.tif')
        self.assertTrue(os.path.exists(input_raster))

        alg = QgsApplication.processingRegistry().createAlgorithmById('grass7:r.neighbors')
        self.assertIsNotNone(alg)

        temp_file = os.path.join(self.temp_dir, 'grass_output.tif')

        # Test an even integer for neighborhood size
        parameters = {'input': input_raster,
                      'selection': None,
                      'method': 0,
                      'size': 4,
                      'gauss': None,
                      'quantile': '',
                      '-c': False,
                      '-a': False,
                      'weight': '',
                      'output': temp_file,
                      'GRASS_REGION_PARAMETER': None,
                      'GRASS_REGION_CELLSIZE_PARAMETER': 0,
                      'GRASS_RASTER_FORMAT_OPT': '',
                      'GRASS_RASTER_FORMAT_META': ''}

        ok, msg = alg.checkParameterValues(parameters, context)
        self.assertFalse(ok)

    def testWatershed(self):
        context = QgsProcessingContext()
        input_raster = os.path.join(testDataPath, 'dem.tif')
        self.assertTrue(os.path.exists(input_raster))

        alg = QgsApplication.processingRegistry().createAlgorithmById('grass7:r.watershed')
        self.assertIsNotNone(alg)

        temp_file = os.path.join(self.temp_dir, 'accumulation.tif')

        # Test non positive threshold integer
        parameters = {'elevation': input_raster,
                      'accumulation': temp_file,
                      'depression': None,
                      'flow': None,
                      'disturbed_land': None,
                      'blocking': None,
                      'max_slope_length': None,
                      'threshold': 0,
                      'convergence': 5,
                      'memory': 300,
                      '-s': False,
                      '-m': False,
                      '-4': False,
                      '-a': False,
                      '-b': False,
                      'GRASS_REGION_PARAMETER': None,
                      'GRASS_REGION_CELLSIZE_PARAMETER': 0,
                      'GRASS_RASTER_FORMAT_OPT': '',
                      'GRASS_RASTER_FORMAT_META': ''}

        ok, msg = alg.checkParameterValues(parameters, context)
        self.assertFalse(ok)


if __name__ == '__main__':
    nose2.main()
