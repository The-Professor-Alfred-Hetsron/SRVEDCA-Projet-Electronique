const express = require('express')
const router = express.Router()
const { checkAdmin } = require("../Middleware/AuthMiddlewares")

const { getAllEtudiant, 
    getEtudiantByStatus, 
    updateSt,
    updateAllEtudiant,
    validated,deleteEtudiant, createEtudiant} = require('../Controllers/EtudiantController')


    router.get('/', getAllEtudiant);
    router.get('/attente', getEtudiantByStatus);
    router.post('/create',createEtudiant);
    router.put('/update/:id', updateSt);
    router.put('/validate', validated);
    router.put('/updateAll',  updateAllEtudiant);
    router.delete('/delete/:id',  deleteEtudiant);

module.exports = router