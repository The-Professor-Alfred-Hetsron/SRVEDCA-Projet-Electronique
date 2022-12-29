const mongoose = require('mongoose')

const etudiantSchema = new mongoose.Schema({
    matricule: {
        type: String,
        required: true,
        unique: true,
        minLength: 3,
        uppercase: true,
    },
    nom: {
        type: String,
        required: true,
        uppercase: true,
    },
    classe: {
        type: mongoose.SchemaTypes.ObjectId,
        required: true,
        ref: 'Classe'
    },
    empreinte: {
        type: String,
        required: true,
        unique: true
    },
    imageEmpreinte: {
        type: String, //On va si nécessaire modifier ça pour mettre ObjectId ref "Image"
    },
    voix: {
        type: String
    },
    statut: {
        type: Boolean, //true = enrollement validé, false = enrolement en attente
        required: true,
    }
    
}, { 
    timestamps: { createdAt: 'dateCreation', updatedAt: 'dateModification' } 
    })

module.exports = mongoose.model('Etudiant', etudiantSchema)
