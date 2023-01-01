const express = require('express')
const router = express.Router()

const CoursController = require('../Controllers/CoursController')

router.post('/store', CoursController.store)
router.get ('/', CoursController.showAll)
router.get ('/showByName', CoursController.showByName)
router.post ('/update', CoursController.update)
router.post('/delete',CoursController.destroy)

module.exports = router