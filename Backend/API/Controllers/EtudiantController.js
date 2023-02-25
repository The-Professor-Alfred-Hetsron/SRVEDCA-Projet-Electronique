const Etudiant = require('../../Database/Etudiant');
const Classe = require('../../Database/Classe');
const mongoose = require('mongoose');
const moment = require('moment/moment');

module.exports.getAllEtudiant = async (req, res) =>{
    const { page } = req.query;
    
    try {
        //const LIMIT = 8;
        //const startIndex = (Number(page) - 1) * LIMIT; // get the starting index of every page
    
        //const total = await Etudiant.countDocuments({});
        //const etudiantList = await Etudiant.find().sort({ _id: -1 }).limit(LIMIT).skip(startIndex);
        const etudiantList = await Etudiant.find().sort({dateCreation: -1}).populate('classe')

        res.json({ etudiantList });
    } catch (error) {    
        res.status(404).json({ message: error.message });
    }
};

module.exports.getEtudiantByStatus = async (req, res) => {
   

    try {
        const etudiants = await Etudiant.find({ statut:false }).populate('classe');

        res.json(etudiants);
    } catch (error) {    
        res.status(404).json({ message: error.message });
    }
};

module.exports.createEtudiant = async (req, res) => {
    const classe = req.body.classe;
    const classEt = await Classe.findOne({nom:classe});
    if(!(classEt instanceof Classe)) return res.status(404).send(`No Class with name : ${req.body.classe}`);
    
    let etudiant = new Etudiant({
        matricule:req.body.matricule,
        nom: req.body.nom,
        classe:classEt._id,
        empreinte: req.body.empreinte ? req.body.empreinte : moment().unix(),
        statut:false
    })

    try {
        let result = await (await etudiant.save()).populate('classe');

        res.status(201).json({message:"Etudiant enregistré avec succès", content: result});
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};


module.exports.updateSt = async (req, res) => {
    //const etudiant = req.body;
    //const {id}=req.params.id
    const {nom, matricule,classe,id}=req.body
    const classeEtudiant = await Classe.findOne({ nom: classe })

    if(!(classeEtudiant instanceof Classe)) return res.status(404).send(`No Class with name : ${req.body.classe}`);

   const student = {
    nom: nom,
    matricule:matricule,
    classe:classeEtudiant._id
   }

    try {
        await Etudiant.findByIdAndUpdate (id, {$set : student});
        let result = await Etudiant.findById(id).populate('classe')

        res.status(201).json({message:"modifié avec succès", content: result});
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};


module.exports.validated = async (req, res) => {
    const { id } = req.body;
  
    //if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No Student with id: ${id}`);

    const updatedData = { statut:true };

    try {
        let result = await Etudiant.findByIdAndUpdate(id, { $set: updatedData })

        res.status(201).json({ message: "Enrollement validé", content: await result.populate('classe')})
    } catch (error) {
        res.status(409).json({ message: error.message })
    }

};


module.exports.updateAllEtudiant = async (req, res) => {
        // create a filter to update all student with statut false
        const filter = { statut: false };
        // set statut to true
        const updateDoc = {
          $set: {
            statut: true,
          },
        };

        try {
            await Etudiant.updateMany(filter, updateDoc);
    
            res.json({ message: "Enrollements validés" });
    
        } catch (error) {
            res.status(409).json({ message: error.message });
        }
};


module.exports.deleteEtudiant  = async (req, res) => {
    const id  = req.params.id;

    //if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No student with id: ${id}`);
    try {
        await Etudiant.findByIdAndRemove(id);

        res.json({ message: "Student deleted successfully." });

    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};