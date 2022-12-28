const express = require('express')
const router = express.Router()

const AdminController = require('../Controllers/AdminController')

router.post('/store', AdminController.store)
router.get('/', AdminController.showAll)
router.post('/update', AdminController.update)
router.post('/destroy', AdminController.destroy)

module.exports = router