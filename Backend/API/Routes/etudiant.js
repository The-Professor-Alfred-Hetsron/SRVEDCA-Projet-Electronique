const express = require('express')
const router = express.Router()
const { checkAdmin } = require("../Middleware/AuthMiddlewares")

const { getAllEtudiant, 
    getEtudiantByStatus, 
    createEtudiant,
    updateAllEtudiant,
    updateEtudiant,deleteEtudiant} = require('../Controllers/EtudiantController')


    router.get('/', checkAdmin, getAllEtudiant);
    router.get('/attente', getEtudiantByStatus);
    router.post('/', createEtudiant);
    router.patch('/:id', checkAdmin, updateEtudiant);
    router.patch('updateAll', checkAdmin, updateAllEtudiant);
    router.delete('/:id', checkAdmin, deleteEtudiant);

module.exports = router