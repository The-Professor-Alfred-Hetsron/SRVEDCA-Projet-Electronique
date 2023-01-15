const express = require('express')
const router = express.Router()

const AdminController = require('../Controllers/AdminController')

router.post('/store', AdminController.store)
router.get('/all', AdminController.showAll)
router.get('/:id', AdminController.showById)
router.put('/update', AdminController.update)
router.delete('/destroy/:id', AdminController.destroy)

module.exports = router