const Etudiant = require('../../Database/Etudiant');
const Classe = require('../../Database/Classe');
const mongoose = require('mongoose')

module.exports.getAllEtudiant = async (req, res) =>{
    const { page } = req.query;
    
    try {
        //const LIMIT = 8;
        //const startIndex = (Number(page) - 1) * LIMIT; // get the starting index of every page
    
        //const total = await Etudiant.countDocuments({});
        //const etudiantList = await Etudiant.find().sort({ _id: -1 }).limit(LIMIT).skip(startIndex);
        const etudiantList = await Etudiant.find().populate('classe')

        res.json({ etudiantList});
    } catch (error) {    
        res.status(404).json({ message: error.message });
    }
};

module.exports.getEtudiantByStatus = async (req, res) => {
   

    try {
        const etudiants = await Etudiant.find({ statut:false }).populate('classe');

        res.json({ data: etudiants });
    } catch (error) {    
        res.status(404).json({ message: error.message });
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

        res.status(201).json({message:"modifié avec succès"});
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};


module.exports.validated = async (req, res) => {
    const { id } = req.body;
  
    if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No Student with id: ${id}`);

    const updatedData = { statut:true };

    await PostMessage.findByIdAndUpdate(id, { $set: updatedData })
    .then(()=>{
        res.json({ message: "Enrollement validé"})
    })
    .catch(error => {
        res.status(409).json({ message: error.message })
    } )


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
    const { id } = req.body;

    //if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No student with id: ${id}`);
    try {
        await Etudiant.findByIdAndRemove(id);

        res.json({ message: "Student deleted successfully." });

    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};