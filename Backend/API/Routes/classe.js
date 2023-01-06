const express = require('express')
const router = express.Router()

const ClasseController = require('../Controllers/ClasseController')

router.post('/store', ClasseController.store)
router.get('/', ClasseController.showAll)
router.post('/update', ClasseController.update)
router.post('/destroy', ClasseController.destroy)

module.exports = router