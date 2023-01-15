const express = require('express')
const router = express.Router()

const ClasseController = require('../Controllers/ClasseController')

router.post('/store', ClasseController.store)
router.get('/all', ClasseController.showAll)
router.get('/:nom', ClasseController.showOne)
router.put('/update', ClasseController.update)
router.delete('/delete/:id', ClasseController.destroy)

module.exports = router